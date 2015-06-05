using System;
using System.Collections.Generic;
using System.Text;
using System.Configuration;
using System.Xml;
using System.Xml.Serialization;

namespace VersionOne.TestComplete.V1Connector {
    [XmlRoot("configuration")]
    public class Configuration {
        [XmlElement("VersionOneConnection")]
        public VersionOneConnectionSettings V1Connection { get; set; }
        [XmlElement("ReferenceFieldName")]
        public string ReferenceName { get; set; }
        [XmlElement("FailedStatusOid")]
        public string FailedOid { get; set; }
        [XmlElement("PassedStatusOid")]
        public string PassedOid { get; set; }
        [XmlElement("ChangeComment")]
        public string ChangeComment { get; set; }
        [XmlElement("DefectDescriptionSuffix")]
        public string DefectDescriptionSuffix { get; set; }
        [XmlElement("CreateDefectMode")]
        public CreateDefect CreateDefectMode { get; set; }

        public static Configuration GetEmptyConfig() {
            Configuration config = new Configuration();
            config.V1Connection = new VersionOneConnectionSettings();
            return config;
        }

        public static Configuration LoadConfiguration() {
            return LoadConfiguration("App.config");
        }

        public static Configuration LoadConfiguration(string fileName) {
            XmlDocument document = new XmlDocument();
            string path = AppDomain.CurrentDomain.SetupInformation.ApplicationBase + "\\" + fileName;
            document.Load(path);

            XmlReader reader = new XmlNodeReader(document.SelectNodes("configuration")[0]);
            XmlSerializer serializer = new XmlSerializer(typeof(Configuration));
            Configuration config = (Configuration)serializer.Deserialize(reader);
            return config;
        }

        public enum CreateDefect {
            All,
            CurrentIteration,
            None,
        }

        [XmlRoot("VersionOneConnection")]
        public class VersionOneConnectionSettings {
            public string Host { get; set; }
            public string UserName { get; set; }
            public string Password { get; set; }
            public bool IntegratedAuth { get; set; }
        }
    }
}