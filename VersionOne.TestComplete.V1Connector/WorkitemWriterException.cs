using System;

namespace VersionOne.TestComplete.V1Connector {
    public class WorkitemWriterException : Exception {
        public WorkitemWriterException(string message) : base(message) { }
        public WorkitemWriterException(string message, Exception exception) : base(message, exception) { }
    }
}