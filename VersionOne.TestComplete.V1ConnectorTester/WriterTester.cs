using System;
using System.Collections.Generic;
using System.Text;
using NUnit.Framework;
using VersionOne.SDK.APIClient;
using VersionOne.TestComplete.V1Connector;

namespace VersionOne.TestComplete.V1ConnectorTester {

    [TestFixture]
    [Ignore("This is integations tests.")]
    public class WriterTester {

        private const string host = "http://domen/VersionOneTest/";
        private const string user = "admin";
        private const string password = "admin";
        private const string metaUrlSuffix = "meta.v1/";
        private const string dataUrlSuffix = "rest-1.v1/";
        private const string projectToken = "Scope:0";
        private const string passedToken = "TestStatus:129";
        private const string failedToken = "TestStatus:155";
        private const string testReference = "TestComplete.Project.1";
        private const string testSuiteReference = "test.testcomplete.8";

        public Stack<Asset> createdAssets = new Stack<Asset>();

        public Asset Timebox;

        private IMetaModel MetaModel;
        private IServices Services;

        [SetUp]
        public void Connect() {

            V1APIConnector metaConnector = new V1APIConnector(host + metaUrlSuffix, user, password);
            MetaModel = new MetaModel(metaConnector);

            V1APIConnector dataConnector = new V1APIConnector(host + dataUrlSuffix, user, password);
            Services = new Services(MetaModel, dataConnector);
        }

        [Test]
        public void UpdatePassedTestStatusTest() {
            Asset test = CreateTest();
            UpdateTestStatus(test, TestResult.TestRunState.Passed, null);

            AssetList assetList = GetTest(test.Oid, passedToken);
            Assert.AreEqual(1, assetList.Count);
        }

        [Test]
        public void UpdateFailedTestStatusTest() {
            Asset test = CreateTest();
            UpdateTestStatus(test, TestResult.TestRunState.Failed, null);

            AssetList assetList = GetTest(test.Oid, failedToken);
            Assert.AreEqual(1, assetList.Count);
        }

        [Test]
        public void CreateDefectForClosedTestTest() {
            IOperation deleteOperation = MetaModel.GetOperation("Test.QuickClose");
            IOperation reactivateOperation = MetaModel.GetOperation("Test.Reactivate");
            IOperation deleteDefectOperation = MetaModel.GetOperation("Defect.Delete");
            
            Asset test = CreateTest();            
            Services.ExecuteOperation(deleteOperation, test.Oid);
            UpdateTestStatus(test, TestResult.TestRunState.Failed, null);
            // reactivate test to delete it at the end of test suit
            Services.ExecuteOperation(reactivateOperation, test.Oid);

            AssetList assets = GetAffectedDefects(test);

            Assert.AreEqual(1, assets.Count);
            //delete created defect
            Services.ExecuteOperation(deleteDefectOperation, assets[0].Oid);
        }

        [Test]
        public void NoCreateDefectForClosedTestTest() {
            IOperation deleteOperation = MetaModel.GetOperation("Test.QuickClose");
            IOperation reactivateOperation = MetaModel.GetOperation("Test.Reactivate");

            Asset test = CreateTest();
            Configuration config = GetConfig();
            config.CreateDefectMode = Configuration.CreateDefect.None;
            Services.ExecuteOperation(deleteOperation, test.Oid);
            UpdateTestStatus(test, TestResult.TestRunState.Failed, config);
            // reactivate test to delete it at the end of test suit
            Services.ExecuteOperation(reactivateOperation, test.Oid);

            AssetList assets = GetAffectedDefects(test);
            Assert.AreEqual(0, assets.Count);
        }

        [Test]
        public void NoCreateDefectForClosedTestTest2() {
            IOperation deleteOperation = MetaModel.GetOperation("Test.QuickClose");
            IOperation reactivateOperation = MetaModel.GetOperation("Test.Reactivate");

            Asset test = CreateTest();
            Configuration config = GetConfig();
            config.CreateDefectMode = Configuration.CreateDefect.CurrentIteration;
            Services.ExecuteOperation(deleteOperation, test.Oid);
            UpdateTestStatus(test, TestResult.TestRunState.Failed, config);
            // reactivate test to delete it at the end of test suit
            Services.ExecuteOperation(reactivateOperation, test.Oid);

            AssetList assets = GetAffectedDefects(test);
            Assert.AreEqual(0, assets.Count);
        }

        [Test]
        public void CreateDefectForClosedTestInCurrentIterationTest() {
            IOperation deleteOperation = MetaModel.GetOperation("Test.QuickClose");
            IOperation reactivateOperation = MetaModel.GetOperation("Test.Reactivate");
            IOperation deleteDefectOperation = MetaModel.GetOperation("Defect.Delete");

            IAssetType typeDefect = MetaModel.GetAssetType("Defect");
            IAttributeDefinition defectScopeScheduleAttDef = typeDefect.GetAttributeDefinition("Scope.Schedule");
            IAttributeDefinition defectTimeboxAttDef = typeDefect.GetAttributeDefinition("Timebox");
            IAssetType typeTest = MetaModel.GetAssetType("Test");
            IAttributeDefinition testScopeScheduleAttDef = typeTest.GetAttributeDefinition("Scope.Schedule");

            Asset scope = CreateProject(null);
            Asset test = CreateTest(scope);

            Configuration config = GetConfig();
            config.CreateDefectMode = Configuration.CreateDefect.CurrentIteration;
            Services.ExecuteOperation(deleteOperation, test.Oid);
            UpdateTestStatus(test, TestResult.TestRunState.Failed, config);
            // reactivate test to delete it at the end of test suit
            Services.ExecuteOperation(reactivateOperation, test.Oid);

            AssetList assets = GetAffectedDefects(test);
            AssetList updateTest = GetTest(test.Oid, passedToken);

            Assert.AreEqual(1, assets.Count);
            Assert.AreEqual(Timebox.Oid.Momentless, assets[0].GetAttribute(defectTimeboxAttDef).Value);
            Assert.AreEqual(((Oid)updateTest[0].GetAttribute(testScopeScheduleAttDef).Value).Momentless, assets[0].GetAttribute(defectScopeScheduleAttDef).Value);
            //delete created defect
            Services.ExecuteOperation(deleteDefectOperation, assets[0].Oid);
        }

        [Test]
        public void CreateDefectForClosedTestInSameTimeboxTest() {
            IOperation deleteOperation = MetaModel.GetOperation("Test.QuickClose");
            IOperation reactivateOperation = MetaModel.GetOperation("Test.Reactivate");
            IOperation deleteDefectOperation = MetaModel.GetOperation("Defect.Delete");

            IAssetType typeDefect = MetaModel.GetAssetType("Defect");
            IAttributeDefinition defectScopeScheduleAttDef = typeDefect.GetAttributeDefinition("Scope.Schedule");
            IAttributeDefinition defectTimeboxAttDef = typeDefect.GetAttributeDefinition("Timebox");
            IAssetType typeTest = MetaModel.GetAssetType("Test");
            IAttributeDefinition testScopeScheduleAttDef = typeTest.GetAttributeDefinition("Scope.Schedule");

            Asset scope = CreateProject(null);
            Asset test = CreateTest(scope);

            Configuration config = GetConfig();
            config.CreateDefectMode = Configuration.CreateDefect.All;
            Services.ExecuteOperation(deleteOperation, test.Oid);
            UpdateTestStatus(test, TestResult.TestRunState.Failed, config);
            // reactivate test to delete it at the end of test suit
            Services.ExecuteOperation(reactivateOperation, test.Oid);

            AssetList assets = GetAffectedDefects(test);
            AssetList updateTest = GetTest(test.Oid, passedToken);

            Assert.AreEqual(1, assets.Count);
            Assert.AreEqual(Timebox.Oid.Momentless, assets[0].GetAttribute(defectTimeboxAttDef).Value);
            Assert.AreEqual(((Oid)updateTest[0].GetAttribute(testScopeScheduleAttDef).Value).Momentless, assets[0].GetAttribute(defectScopeScheduleAttDef).Value);
            //delete created defect
            Services.ExecuteOperation(deleteDefectOperation, assets[0].Oid);
        }

        [Test]
        public void TestRunCreationTest() {
            int elapsedSecond = 125;
            int failed = 5;
            int notRun = 6;
            int passed = 7;
            string testRunName = "test runned test in TC8";
            string testRunDesription = "test run description";
            Asset testSuit = CreateProjectWithTestSuite();

            SuiteResult suiteResult = new SuiteResult();
            suiteResult.ElapsedSeconds = elapsedSecond;
            suiteResult.FailedTestCount = failed;
            suiteResult.Name = testRunName;
            suiteResult.NotRunTestCount = notRun;
            suiteResult.PassedTestCount = passed;
            suiteResult.Reference = testSuiteReference;

            WorkitemWriter writer = new WorkitemWriter();
            //doing publish data to VersionOne twice
            writer.Publish(suiteResult);
            writer.Publish(suiteResult);

            Reconnect();

            IOperation deleteOperation = MetaModel.GetOperation("Story.Delete");
            IAssetType typeTestRun = MetaModel.GetAssetType("TestRun");
            IAttributeDefinition testRunNameAttDef = typeTestRun.GetAttributeDefinition("Name");
            IAttributeDefinition testRunTestSuiteAttDef = typeTestRun.GetAttributeDefinition("TestSuite");
            IAttributeDefinition testRunFailedAttDef = typeTestRun.GetAttributeDefinition("Failed");
            IAttributeDefinition testRunPassedAttDef = typeTestRun.GetAttributeDefinition("Passed");
            IAttributeDefinition testRunNotRunAttDef = typeTestRun.GetAttributeDefinition("NotRun");
            IAttributeDefinition testRunElapsedAttDef = typeTestRun.GetAttributeDefinition("Elapsed");
            IAttributeDefinition testRunDescriptionAttDef = typeTestRun.GetAttributeDefinition("Description");

            Query query = new Query(typeTestRun);
            query.Selection.Add(testRunNameAttDef);
            query.Selection.Add(testRunFailedAttDef);
            query.Selection.Add(testRunPassedAttDef);
            query.Selection.Add(testRunNotRunAttDef);
            query.Selection.Add(testRunElapsedAttDef);
            query.Selection.Add(testRunDescriptionAttDef);            
            FilterTerm termTestRun = new FilterTerm(testRunTestSuiteAttDef);
            termTestRun.Equal(testSuit.Oid);
            query.Filter = termTestRun;
            AssetList assetList = Services.Retrieve(query).Assets;

            Assert.AreEqual(2, assetList.Count);
            // check fields value and delete TestRuns from the VersionOne
            foreach (Asset runTest in assetList) {
                Assert.AreEqual(failed, runTest.GetAttribute(testRunFailedAttDef).Value);
                Assert.AreEqual(notRun, runTest.GetAttribute(testRunNotRunAttDef).Value);
                Assert.AreEqual(passed, runTest.GetAttribute(testRunPassedAttDef).Value);
                Assert.AreEqual(elapsedSecond, runTest.GetAttribute(testRunElapsedAttDef).Value);
                Assert.AreEqual(testRunName, runTest.GetAttribute(testRunNameAttDef).Value);
                Assert.AreEqual(testRunDesription, runTest.GetAttribute(testRunDescriptionAttDef).Value);
                
                Services.ExecuteOperation(deleteOperation, runTest.Oid);
            }
        }

        [Test]
        [ExpectedException(typeof(WorkitemWriterException))]
        public void IncorrectTestStatus1Test() {
            Configuration config = GetConfig();
            config.FailedOid = "TestStatus:0";
            WorkitemWriter writer = new WorkitemWriter(config);
        }

        [Test]
        [ExpectedException(typeof(WorkitemWriterException))]
        public void IncorrectTestStatus2Test() {
            Configuration config = GetConfig();
            config.FailedOid = "TTT:0";
            WorkitemWriter writer = new WorkitemWriter(config);
        }

        [Test]
        [ExpectedException(typeof(WorkitemWriterException))]
        public void IncorrectTestStatus3Test() {
            Configuration config = GetConfig();
            config.FailedOid = "Scope:0";
            WorkitemWriter writer = new WorkitemWriter(config);
        }

        #region support methods
        private Asset CreateTest() {
            return CreateTest(null);
        }

        private Asset CreateTest(Asset scope) {
            string testName = "TC 8 test (new one)";
            string storyName = "TC 8 story (new one)";

            IAssetType typeTest = MetaModel.GetAssetType("Test");
            IAttributeDefinition testNameAttDef = typeTest.GetAttributeDefinition("Name");
            IAttributeDefinition testParentAttDef = typeTest.GetAttributeDefinition("Parent");
            IAttributeDefinition testReferenceAttDef = typeTest.GetAttributeDefinition("Reference");
            IAttributeDefinition testStatusAttDef = typeTest.GetAttributeDefinition("Status");
            IAttributeDefinition testTimeboxAttDef = typeTest.GetAttributeDefinition("Timebox");

            IAssetType typeStory = MetaModel.GetAssetType("Story");
            IAttributeDefinition storyNameAttDef = typeStory.GetAttributeDefinition("Name");
            IAttributeDefinition storyScopeAttDef = typeStory.GetAttributeDefinition("Scope");
            IAttributeDefinition storyTimeboxAttDef = typeStory.GetAttributeDefinition("Timebox");
            Oid projectId;
            if (scope == null) {
                projectId = Oid.FromToken(projectToken, MetaModel);
            } else {
                projectId = scope.Oid;
            }

            Asset story = new Asset(typeStory);
            story.SetAttributeValue(storyNameAttDef, storyName);
            story.SetAttributeValue(storyScopeAttDef, projectId);
            if (Timebox != null) {
                story.SetAttributeValue(storyTimeboxAttDef, Timebox.Oid);
            }
            SaveAsset(story);

            Asset test = new Asset(typeTest);
            test.SetAttributeValue(testNameAttDef, testName);
            test.SetAttributeValue(testParentAttDef, story.Oid);
            test.SetAttributeValue(testReferenceAttDef, testReference);
            

            SaveAsset(test);

            return test;
        }


        private Asset CreateProject(Asset testSuite) {
            string scheduleName = "test schedule";
            string scopeName = "___TestComplete8_testing";
            string timeboxName = "test timebox";

            IAssetType typeSchedule = MetaModel.GetAssetType("Schedule");
            IAttributeDefinition scheduleNameAttDef = typeSchedule.GetAttributeDefinition("Name");
            IAttributeDefinition scheduleTimbeboxLenghtAttDef = typeSchedule.GetAttributeDefinition("TimeboxLength");
            IAttributeDefinition scheduleTimbeboxGapAttDef = typeSchedule.GetAttributeDefinition("TimeboxGap");

            IAssetType typeScope = MetaModel.GetAssetType("Scope");
            IAttributeDefinition scopeNameAttDef = typeScope.GetAttributeDefinition("Name");
            IAttributeDefinition scopeParentScopeAttDef = typeScope.GetAttributeDefinition("Parent");
            IAttributeDefinition scopeTestSuiteAttDef = typeScope.GetAttributeDefinition("TestSuite");
            IAttributeDefinition scopeBeginDateAttDef = typeScope.GetAttributeDefinition("BeginDate");
            IAttributeDefinition scopeScheduleAttDef = typeScope.GetAttributeDefinition("Schedule");

            Asset schedule = new Asset(typeSchedule);
            schedule.SetAttributeValue(scheduleNameAttDef, scheduleName);
            schedule.SetAttributeValue(scheduleTimbeboxLenghtAttDef, "2 Days");
            schedule.SetAttributeValue(scheduleTimbeboxGapAttDef, "0 Days");
            SaveAsset(schedule);

            IAssetType typeTimebox = MetaModel.GetAssetType("Timebox");
            IAttributeDefinition timeboxNameAttDef = typeTimebox.GetAttributeDefinition("Name");
            IAttributeDefinition timeboxEndDateAttDef = typeTimebox.GetAttributeDefinition("EndDate");
            IAttributeDefinition timeboxBeginDateAttDef = typeTimebox.GetAttributeDefinition("BeginDate");
            IAttributeDefinition timeboxStateAttDef = typeTimebox.GetAttributeDefinition("State");
            IAttributeDefinition timeboxScheduleAttDef = typeTimebox.GetAttributeDefinition("Schedule");

            Timebox = new Asset(typeTimebox);
            Timebox.SetAttributeValue(timeboxNameAttDef, timeboxName);
            Timebox.SetAttributeValue(timeboxBeginDateAttDef, DateTime.Now.Date);
            Timebox.SetAttributeValue(timeboxEndDateAttDef, DateTime.Now.Date.AddDays(2));
            Timebox.SetAttributeValue(timeboxStateAttDef, "State:101");
            Timebox.SetAttributeValue(timeboxScheduleAttDef, schedule.Oid);

            SaveAsset(Timebox);

            Asset scope = new Asset(typeScope);
            scope.SetAttributeValue(scopeNameAttDef, scopeName);
            if (testSuite != null) {
                scope.SetAttributeValue(scopeTestSuiteAttDef, testSuite.Oid);
            }
            scope.SetAttributeValue(scopeParentScopeAttDef, projectToken);
            scope.SetAttributeValue(scopeBeginDateAttDef, DateTime.Now);
            scope.SetAttributeValue(scopeScheduleAttDef, schedule.Oid);
            SaveAsset(scope);

            return scope;
        }

        private Asset CreateProjectWithTestSuite() {
            string testSuiteName = "test_testcomplete_8";            

            IAssetType typeTestSuite = MetaModel.GetAssetType("TestSuite");
            IAttributeDefinition testSuiteNameAttDef = typeTestSuite.GetAttributeDefinition("Name");
            IAttributeDefinition testSuiteReferenceAttDef = typeTestSuite.GetAttributeDefinition("Reference"); 

            Asset testSuite = new Asset(typeTestSuite);
            testSuite.SetAttributeValue(testSuiteNameAttDef, testSuiteName);
            testSuite.SetAttributeValue(testSuiteReferenceAttDef, testSuiteReference);
            SaveAsset(testSuite);

            CreateProject(testSuite);

            return testSuite;
        }

        private void SaveAsset(Asset asset) {
            Services.Save(asset);
            createdAssets.Push(asset);
        }

        private AssetList GetTest(Oid testOid, string statusToken) {
            IAssetType typeTest = MetaModel.GetAssetType("Test");
            IAttributeDefinition testNameAttDef = typeTest.GetAttributeDefinition("Name");
            IAttributeDefinition testParentAttDef = typeTest.GetAttributeDefinition("Parent");
            IAttributeDefinition testReferenceAttDef = typeTest.GetAttributeDefinition("Reference");
            IAttributeDefinition testStatusAttDef = typeTest.GetAttributeDefinition("Status");
            IAttributeDefinition testScopeScheduleAttDef = typeTest.GetAttributeDefinition("Scope.Schedule");
            Query query = new Query(typeTest);
            query.Selection.Add(testStatusAttDef);
            query.Selection.Add(testScopeScheduleAttDef);            

            List<FilterTerm> terms = new List<FilterTerm>(2);
            FilterTerm termKey = new FilterTerm(typeTest.GetAttributeDefinition("Key"));
            termKey.Equal(testOid.Key);
            FilterTerm termStatus = new FilterTerm(testStatusAttDef);
            termStatus.Equal(statusToken);
            terms.Add(termKey);
            terms.Add(termStatus);

            query.Filter = new AndFilterTerm(terms.ToArray());
            return Services.Retrieve(query).Assets;
        }

        private AssetList GetAffectedDefects(Asset test) {
            IAssetType typeTest = MetaModel.GetAssetType("Test");
            IAssetType typeDefect = MetaModel.GetAssetType("Defect");
            IAttributeDefinition testParentAttDef = typeTest.GetAttributeDefinition("Parent");
            IAttributeDefinition defectAffectedPrimaryWorkitemsAttDef = typeDefect.GetAttributeDefinition("AffectedPrimaryWorkitems");
            IAttributeDefinition defectScopeScheduleAttDef = typeDefect.GetAttributeDefinition("Scope.Schedule");
            IAttributeDefinition defectTimeboxAttDef = typeDefect.GetAttributeDefinition("Timebox");

            Query query = new Query(typeDefect);
            query.Selection.Add(defectScopeScheduleAttDef);
            query.Selection.Add(defectTimeboxAttDef);
            FilterTerm term = new FilterTerm(defectAffectedPrimaryWorkitemsAttDef);
            term.Equal(test.GetAttribute(testParentAttDef).Value);
            query.Filter = term;
            return Services.Retrieve(query).Assets;
        }

        private void UpdateTestStatus(Asset test, TestResult.TestRunState status, Configuration config) {            
            if (config == null) {
                config = GetConfig();
            }
            Oid testOid = test.Oid;

            TestResult testResult = new TestResult();
            testResult.ElapsedSeconds = 20;
            testResult.Reference = testReference;
            testResult.RunTime = DateTime.Now;
            testResult.State = status;
            SuiteResult suiteResult = new SuiteResult();
            suiteResult.TestResults.Add(testResult);

            WorkitemWriter writer = new WorkitemWriter(config);
            writer.Publish(suiteResult);

            Reconnect();
        }

        private void DeleteCreatedAssets() {
            IOperation deleteOperation = MetaModel.GetOperation("Test.Delete");
            while (createdAssets.Count > 0) {
                Asset asset = createdAssets.Pop();
                Services.ExecuteOperation(deleteOperation, asset.Oid);
            }
        }

        private void Reconnect() {
            MetaModel = null;
            Services = null;

            Connect();
        }

        private Configuration GetConfig() {
            Configuration config = Configuration.GetEmptyConfig();
            config.V1Connection.Host = host;
            config.V1Connection.UserName = user;
            config.V1Connection.Password = password;
            config.V1Connection.IntegratedAuth = false;
            config.ReferenceName = "Reference";
            config.FailedOid = "TestStatus:155";
            config.PassedOid = "TestStatus:129";
            config.ChangeComment = "test comment";
            config.DefectDescriptionSuffix = "Created by TestComplete 8";
            config.CreateDefectMode = Configuration.CreateDefect.All;
            return config;
        }

        #endregion

        [TearDown]
        public void TearDown() {
            DeleteCreatedAssets();
        }
    }
}
