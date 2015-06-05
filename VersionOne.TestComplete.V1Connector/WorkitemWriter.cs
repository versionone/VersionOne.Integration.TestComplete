using System;
using System.Collections.Generic;
using VersionOne.SDK.APIClient;
using Attribute = VersionOne.SDK.APIClient.Attribute;
using log4net.Config;
using System.IO;

namespace VersionOne.TestComplete.V1Connector {
    public class WorkitemWriter : IWorkitemWriter {
        private const string MetaUrlSuffix = "meta.v1/";
        private const string DataUrlSuffix = "rest-1.v1/";
        private const string LocalizerUrlSuffix = "loc.v1/";

        private readonly Configuration config;
        private readonly Logger log = new Logger("WorkitemWriter");

        private IMetaModel MetaModel;
        private IServices Services;
        private ILocalizer Localizer;
        private readonly IDictionary<TestResult.TestRunState, Oid> testStatuses = new Dictionary<TestResult.TestRunState, Oid>();
        private V1Attributes attributes;

        public WorkitemWriter() : this(Configuration.LoadConfiguration()) { }

        public WorkitemWriter(Configuration config) {
            this.config = config;
        }

        /// <summary>
        /// Processing and publishing info about test and test runs to VersionOne.
        /// </summary>
        /// <param name="suiteResult">Information about tests</param>
        public void Publish(SuiteResult suiteResult) {
            if (suiteResult.TestResults.Count == 0) {
                log.Info("There are no completed tests. Stopping further processing ...");
                return;
            }

            log.Info(string.Format("{0} test(s) finished. Processing...", suiteResult.TestResults.Count));
            
            foreach(TestResult testResult in suiteResult.TestResults) {
                UpdateTest(testResult);
            }

            log.Info(string.Format("Creating Test Runs for Test Suite with {0} reference...", suiteResult.Reference));
            
            CreateTestRuns(suiteResult);
            
            log.Info(string.Format("Creation of Test Run for Test Suite with {0} reference finished.", suiteResult.Reference));
        }

        /// <summary>
        /// Read configuration, create connection to VersionOne.
        /// </summary>
        public void Connect() {
            XmlConfigurator.Configure(new FileInfo(AppDomain.CurrentDomain.SetupInformation.ApplicationBase + "\\log4net.config"));
            log.Info("Initializing VersionOne connection...");
            log.Info("Configuration loaded.");
            ConnectValidate();
            CreateConnection();
            log.Info("Connection validated.");

            attributes = new V1Attributes(MetaModel);
            LoadTestStatuses(TestResult.TestRunState.Failed, config.FailedOid);
            LoadTestStatuses(TestResult.TestRunState.Passed, config.PassedOid);
        }

        /// <summary>
        /// Update tests.
        /// </summary>
        /// <param name="testResult">Test run result.</param>
        private void UpdateTest(TestResult testResult) {
            QueryResult relatedTests = GetRelatedTests(testResult.Reference);

            Oid statusOid;
            
            if(!testStatuses.TryGetValue(testResult.State, out statusOid)) {
                statusOid = Oid.Null;
            }

            foreach(Asset test in relatedTests.Assets) {
                Attribute stateAttribute = test.GetAttribute(attributes.TestAssetState);
                
                if(((AssetState) stateAttribute.Value) == AssetState.Active) {
                    UpdateTestStatus(statusOid, test);
                } else if (ShouldCreateDefect(test) && testResult.State == TestResult.TestRunState.Failed) {
                    CreateDefectForClosedTest(testResult, test);
                }
            }
        }

        /// <summary>
        /// Should defect to be created?
        /// Depends on:
        ///             user configuration.
        ///             are there any already related defects.
        /// </summary>
        /// <param name="test">specified test</param>
        private bool ShouldCreateDefect(Asset test) {
            if(RelatedDefectExists(test)) {
                return false;
            }

            switch (config.CreateDefectMode) {
                case Configuration.CreateDefect.None:
                    return false;
                case Configuration.CreateDefect.All:
                    return true;
                case Configuration.CreateDefect.CurrentIteration:
                    return IsTimeboxCurrent(test);
                default:
                    throw new NotSupportedException();
            }
        }

        private bool IsTimeboxCurrent(Asset test) {
            Oid timeboxOid = test.GetAttribute(attributes.TestTimeBox).Value as Oid;
            
            if(timeboxOid == null || timeboxOid.IsNull) {
                return false;
            }

            Query timeboxStateQuery = new Query(attributes.GetAssetType(V1Attributes.TimeboxType));
            timeboxStateQuery.Selection.Add(attributes.TimeboxAssetState);
            FilterTerm term = new FilterTerm(attributes.TimeBoxId);
            term.Equal(timeboxOid.Token);
            timeboxStateQuery.Filter = term;
            QueryResult result = Services.Retrieve(timeboxStateQuery);

            if(result.Assets.Count == 0) {
                return false;
            }
            
            Asset timebox = result.Assets[0];
            return ((AssetState) timebox.GetAttribute(attributes.TimeboxAssetState).Value == AssetState.Active);
        }

        /// <summary>
        /// Check if defect related to specified test already exists.
        /// </summary>
        /// <param name="test">test</param>
        private bool RelatedDefectExists(Asset test) {
            IAttributeDefinition testParentAffByDef = attributes.TestParentAffectedByDefects;
            return (test.GetAttribute(testParentAffByDef) != null) &&
                        (test.GetAttribute(testParentAffByDef).ValuesList.Count > 0);
        }

        /// <summary>
        /// Create defect for closed tests.
        /// </summary>
        /// <param name="testResult">Test run result.</param>
        /// <param name="test">Closed test.</param>
        private void CreateDefectForClosedTest(TestResult testResult, Asset test) {
            Oid parent = test.GetAttribute(attributes.TestParent).Value as Oid;
            string defectName = string.Format("{0} \"{1}\" has failing Acceptance Test(s)",
			                         	Localizer.Resolve(parent.AssetType.DisplayName),
                                        test.GetAttribute(attributes.TestParentNumber).Value);
            string defectDescription = string.Format(
                    "One or more acceptance tests failed at \"{0}\".<BR />{1}",
                    testResult.RunTime, config.DefectDescriptionSuffix);

            //TODO add information about failed test

            Asset defect = Services.New(attributes.GetAssetType(V1Attributes.DefectType), Oid.Null);
            defect.AddAttributeValue(attributes.DefectAffectedPrimaryWorkitems, test.GetAttribute(attributes.TestParent).Value);
            defect.SetAttributeValue(attributes.DefectScope, test.GetAttribute(attributes.TestScope).Value);
            defect.SetAttributeValue(attributes.DefectName, defectName);
            defect.SetAttributeValue(attributes.DefectDescription, defectDescription);

            Oid timeboxOid = FindTimebox(test);
            
            if(!timeboxOid.IsNull) {
                defect.SetAttributeValue(attributes.DefectTimebox, timeboxOid);
            }

            Services.Save(defect, config.ChangeComment);
            log.Info(string.Format("Defect has been created for {0} test", testResult.Reference));
        }

        private Oid FindTimebox(Asset test) {
            if(config.CreateDefectMode == Configuration.CreateDefect.CurrentIteration) {
                return test.GetAttribute(attributes.TestTimeBox).Value as Oid;
            }
            
            Oid scheduleOid = test.GetAttribute(attributes.TestScopeSchedule).Value as Oid;

            if(scheduleOid == null || scheduleOid.IsNull) {
                return Oid.Null;
            }

            return GetTimeBox(scheduleOid);
        }

        private Oid GetTimeBox(Oid scheduleOid) {            
            Query query = new Query(attributes.GetAssetType(V1Attributes.TimeboxType));
            
            FilterTerm scheduleTerm = new FilterTerm(attributes.TimeboxSchedule);
            scheduleTerm.Equal(scheduleOid);
            FilterTerm assetStateTerm = new FilterTerm(attributes.TimeboxAssetState);
            assetStateTerm.Equal(AssetState.Active);
            query.Filter = new AndFilterTerm(scheduleTerm, assetStateTerm);
            query.OrderBy.MajorSort(attributes.TimeboxEndDate, OrderBy.Order.Ascending);
            query.Paging = new Paging(0, 1);
            
            QueryResult result = Services.Retrieve(query);

            if (result.Assets.Count != 0) {
                return result.Assets[0].Oid;
            }
            
            return Oid.Null;
        }

        /// <summary>
        /// Create Test Runs in VersionOne.
        /// </summary>
        /// <param name="suiteResult">Information from TestComplete.</param>
        private void CreateTestRuns(SuiteResult suiteResult) {
            if(string.IsNullOrEmpty(suiteResult.Reference)) {
                log.Info("Suite Reference is null or empty. Skipping...");
                return;
            }

            Query query = new Query(attributes.GetAssetType(V1Attributes.TestSuiteType));
            FilterTerm term = new FilterTerm(attributes.TestSuiteReference);
            term.Equal(suiteResult.Reference);
            query.Filter = term;
            QueryResult result = Services.Retrieve(query);

            if (result.Assets.Count == 0) {
                log.Info(string.Format("There are no Test Suites with {0} reference.", suiteResult.Reference));
                return;
            }
            
            AssetList newTestRuns = new AssetList();

            foreach (Asset testSuite in result.Assets) {
                Asset testRun = Services.New(attributes.GetAssetType(V1Attributes.TestRunType), testSuite.Oid);

                testRun.SetAttributeValue(attributes.TestRunName, suiteResult.Name);
                testRun.SetAttributeValue(attributes.TestRunDescription, SuiteResult.Description);
                testRun.SetAttributeValue(attributes.TestRunDate, suiteResult.RunTime);
                testRun.SetAttributeValue(attributes.TestRunPassed, suiteResult.PassedTestCount);
                testRun.SetAttributeValue(attributes.TestRunFailed, suiteResult.FailedTestCount);
                testRun.SetAttributeValue(attributes.TestRunNotRun, suiteResult.NotRunTestCount);
                testRun.SetAttributeValue(attributes.TestRunElapsed, suiteResult.ElapsedSeconds);
                
                newTestRuns.Add(testRun);
            }

            if(newTestRuns.Count != 0) {
                Services.Save(newTestRuns);
                log.Info(string.Format("Saving Test Run {0}.", suiteResult.Name));
            }
        }

        /// <summary>
        /// Update test statuses in VersionOne.
        /// </summary>
        /// <param name="newStatusOid">Test Status OID for update.</param>
        /// <param name="test">Test to update.</param>
        private void UpdateTestStatus(Oid newStatusOid, Asset test) {
            Attribute statusAttribute = test.GetAttribute(attributes.TestStatus);
            Oid statusOid = (Oid) statusAttribute.Value;

            if(newStatusOid != statusOid) {
                test.SetAttributeValue(attributes.TestStatus, newStatusOid);
                Services.Save(test, config.ChangeComment);
                log.Info(string.Format("Updating status of Acceptance Test \"{0}\".", test.Oid.Token));
            }
        }

        /// <summary>
        /// Find related tests with specific reference.
        /// </summary>
        /// <param name="reference">Reference used to connect VersionOne tests and TestComplete test items</param>
        /// <returns>Found tests.</returns>
        private QueryResult GetRelatedTests(string reference) {
            Query testQuery = new Query(attributes.GetAssetType(V1Attributes.TestType));
            
            testQuery.Selection.Add(attributes.TestAssetState);
            testQuery.Selection.Add(attributes.TestStatus);
            testQuery.Selection.Add(attributes.TestParentAffectedByDefects);
            testQuery.Selection.Add(attributes.TestParent);
            testQuery.Selection.Add(attributes.TestScope);
            testQuery.Selection.Add(attributes.TestScopeSchedule);
            testQuery.Selection.Add(attributes.TestTimeBox);

            IAttributeDefinition userField = attributes.GetAttrDef(V1Attributes.TestType, config.ReferenceName);
            FilterTerm term = new FilterTerm(userField);
            term.Equal(reference);
            testQuery.Filter = term;

            return Services.Retrieve(testQuery);
        }

        /// <summary>
        /// Load Assets with Test Statuses from VersionOne.
        /// </summary>
        /// <param name="state">Name of status</param>
        /// <param name="token">Test Status token in VersionOne</param>
        private void LoadTestStatuses(TestResult.TestRunState state, string token) {
            IAssetType typeTestStatus = attributes.GetAssetType(V1Attributes.TestStatusType);
            Oid oid = null;

            try {
                oid = Services.GetOid(token);
            } catch (OidException ex) {
                string message = string.Format("{0} is not correct token for test state {1})", token, state);
                log.Fatal(message, ex);
                throw new WorkitemWriterException(message, ex);
            }

            if(oid == null){
                string message = string.Format("{0} is not correct token for test state {1}", token, state);
                log.Fatal(message);
                throw new WorkitemWriterException(message);
            }

            if(oid.AssetType != typeTestStatus) {
                string message = string.Format("{0} is not TestStatus type (State {1})", token, state);
                log.Fatal(message);
                throw new WorkitemWriterException(message);
            }

            Query q = new Query(oid.AssetType);
            FilterTerm term = new FilterTerm(oid.AssetType.GetAttributeDefinition("Key"));
            term.Equal(oid.Key);
            q.Filter = term;
            AssetList assetlist = Services.Retrieve(q).Assets;
            
            if (assetlist.Count == 0) {
                string message = string.Format("{0} does not exist.(for {1} status)", token, state);
                log.Fatal(message);
                throw new WorkitemWriterException(message);
            }

            testStatuses.Add(state, oid);
        }

        #region VersionOne connection
        
        private void CreateConnection() {
            if(!config.V1Connection.Host.EndsWith("/")) {
                config.V1Connection.Host += "/";
            }

            V1APIConnector metaConnector = new V1APIConnector(config.V1Connection.Host + MetaUrlSuffix, config.V1Connection.UserName, config.V1Connection.Password, config.V1Connection.IntegratedAuth);
            MetaModel = new MetaModel(metaConnector);

            V1APIConnector dataConnector = new V1APIConnector(config.V1Connection.Host + DataUrlSuffix, config.V1Connection.UserName, config.V1Connection.Password, config.V1Connection.IntegratedAuth);
            Services = new Services(MetaModel, dataConnector);

            V1APIConnector localizerConnector = new V1APIConnector(config.V1Connection.Host + LocalizerUrlSuffix, config.V1Connection.UserName, config.V1Connection.Password, config.V1Connection.IntegratedAuth);
            Localizer = new Localizer(localizerConnector);
        }

        private void ConnectValidate() {
            V1ConnectionValidator validator;
            
            if(!config.V1Connection.IntegratedAuth) {
                validator = new V1ConnectionValidator(config.V1Connection.Host, config.V1Connection.UserName, config.V1Connection.Password, config.V1Connection.IntegratedAuth);
            } else {
                validator = new V1ConnectionValidator(config.V1Connection.Host, null, null, config.V1Connection.IntegratedAuth);
            }

            try {
                validator.CheckConnection();
                validator.CheckAuthentication();
            } catch (ConnectionException ex) {
                log.Fatal("Connection is not valid", ex);
                throw new WorkitemWriterException("Connection is not valid", ex);
            }
        }

        #endregion
    }
}