using System;
using System.Collections.Generic;
using System.Text;
using NUnit.Framework;
using VersionOne.TestComplete.V1Connector;
using VersionOne.SDK.APIClient;

namespace VersionOne.TestComplete.V1ConnectorTester {
    [TestFixture]
    public class ConfigurationTester {
        private const string TestConfigName = "TestConfig.config";

        [Test]
        public void LoadConfigFileTest() {
            Configuration config = Configuration.LoadConfiguration(TestConfigName);

            Assert.AreEqual("http://domen/VersionOne/", config.V1Connection.Host);
            Assert.AreEqual("admin", config.V1Connection.UserName);
            Assert.AreEqual("admin", config.V1Connection.Password);
            Assert.AreEqual(true, config.V1Connection.IntegratedAuth);
            Assert.AreEqual("Reference", config.ReferenceName);
            Assert.AreEqual("TestStatus:155", config.FailedOid);
            Assert.AreEqual("TestStatus:129", config.PassedOid);
            Assert.AreEqual("test comment", config.ChangeComment);
            Assert.AreEqual("Created by TestComplete 8", config.DefectDescriptionSuffix);
            Assert.AreEqual("None", config.CreateDefectMode.ToString());
            
            

        }
    }
}
