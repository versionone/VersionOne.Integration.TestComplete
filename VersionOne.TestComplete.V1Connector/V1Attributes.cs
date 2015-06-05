using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using VersionOne.SDK.APIClient;

namespace VersionOne.TestComplete.V1Connector {
    public class V1Attributes {
        private readonly IMetaModel MetaModel;

        public const string TestType = "Test";
        public const string TimeboxType = "Timebox";
        public const string DefectType = "Defect";
        public const string TestSuiteType = "TestSuite";
        public const string TestRunType = "TestRun";
        public const string TestStatusType = "TestStatus";

        public V1Attributes(IMetaModel metaModel) {
            MetaModel = metaModel;
        }

        #region Test attribute definitions

        public IAttributeDefinition TestAssetState {
            get { return GetAttrDef(TestType, "AssetState"); }
        }

        public IAttributeDefinition TestTimeBox {
            get { return GetAttrDef(TestType, "Timebox"); }
        }

        public IAttributeDefinition TestParentAffectedByDefects {
            get { return GetAttrDef(TestType, "Parent.AffectedByDefects[AssetState='Active']"); }
        }

        public IAttributeDefinition TestParent {
            get { return GetAttrDef(TestType, "Parent"); }
        }

        public IAttributeDefinition TestScope {
            get { return GetAttrDef(TestType, "Scope"); }
        }

        public IAttributeDefinition TestParentNumber {
            get { return GetAttrDef(TestType, "Parent.Number"); }
        }

        public IAttributeDefinition TestScopeSchedule {
            get { return GetAttrDef(TestType, "Scope.Schedule"); }
        }

        public IAttributeDefinition TestStatus {
            get { return GetAttrDef(TestType, "Status"); }
        }

        #endregion

        #region Timebox  attribute definitions

        public IAttributeDefinition TimeboxAssetState {
            get { return GetAttrDef(TimeboxType, "AssetState"); }
        }

        public IAttributeDefinition TimeBoxId {
            get { return GetAttrDef(TimeboxType, "ID"); }
        }

        public IAttributeDefinition TimeboxSchedule {
            get { return GetAttrDef(TimeboxType, "Schedule"); }
        }

        public IAttributeDefinition TimeboxEndDate {
            get { return GetAttrDef(TimeboxType, "EndDate"); }
        }

        #endregion

        #region Defect  attribute definitions

        public IAttributeDefinition DefectAffectedPrimaryWorkitems {
            get { return GetAttrDef(DefectType, "AffectedPrimaryWorkitems"); }
        }

        public IAttributeDefinition DefectName {
            get { return GetAttrDef(DefectType, "Name"); }
        }

        public IAttributeDefinition DefectScope {
            get { return GetAttrDef(DefectType, "Scope"); }
        }

        public IAttributeDefinition DefectTimebox {
            get { return GetAttrDef(DefectType, "Timebox"); }
        }

        public IAttributeDefinition DefectDescription {
            get { return GetAttrDef(DefectType, "Description"); }
        }

        #endregion

        #region Defect attribute definitions

        public IAttributeDefinition TestSuiteReference {
            get { return GetAttrDef(TestSuiteType, "Reference"); }
        }

        #endregion

        #region Test Run attribute definitions

        public IAttributeDefinition TestRunName {
            get { return GetAttrDef(TestRunType, "Name"); }
        }

        public IAttributeDefinition TestRunDescription {
            get { return GetAttrDef(TestRunType, "Description"); }
        }

        public IAttributeDefinition TestRunDate {
            get { return GetAttrDef(TestRunType, "Date"); }
        }

        public IAttributeDefinition TestRunPassed {
            get { return GetAttrDef(TestRunType, "Passed"); }
        }

        public IAttributeDefinition TestRunFailed {
            get { return GetAttrDef(TestRunType, "Failed"); }
        }

        public IAttributeDefinition TestRunNotRun {
            get { return GetAttrDef(TestRunType, "NotRun"); }
        }

        public IAttributeDefinition TestRunElapsed {
            get { return GetAttrDef(TestRunType, "Elapsed"); }
        }

        #endregion

        public IAttributeDefinition GetAttrDef(string type, string attName) {
            return GetAssetType(type.ToString()).GetAttributeDefinition(attName);
        }

        public IAssetType GetAssetType(string type) {
            return MetaModel.GetAssetType(type);
        }
    }
}