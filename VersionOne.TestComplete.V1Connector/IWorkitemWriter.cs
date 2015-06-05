using System;
using System.Collections.Generic;
using System.Text;

namespace VersionOne.TestComplete.V1Connector {
    public interface IWorkitemWriter {
        void Connect();
        void Publish(SuiteResult suiteResult);
    }
}
