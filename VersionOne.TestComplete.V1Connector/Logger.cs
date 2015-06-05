using System;
using log4net;

namespace VersionOne.TestComplete.V1Connector {
    public class Logger {
        private readonly ILog logger;

        public Logger() {
            logger = LogManager.GetLogger("Plugin");
        }

        public Logger(String name) {
            logger = LogManager.GetLogger(name);
        }
        
        public Logger(Type type) {
            logger = LogManager.GetLogger(type);
        }

        public void Fatal(object message, Exception t) {
            try {
                logger.Fatal(message, t);
            } catch {}
        }

        public void Fatal(object message) {
            try {
                logger.Fatal(message);
            } catch {}
        }

        public void Info(object message) {
            try {
                logger.Info(message);
            } catch {}
        }

        public void Debug(object message) {
            try {
                logger.Debug(message);
            } catch { }
        }
    }
}