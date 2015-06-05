using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace VersionOne.TestComplete.V1Connector {
    public abstract class AbstractResult {
        public DateTime RunTime { get; set; }
        public double ElapsedSeconds { get; set; }
        public string Reference { get; set; }

        public AbstractResult() {
            RunTime = DateTime.Now;
        }

        public void RecordEndTime() {
            var endTime = DateTime.Now;
            var difference = endTime - RunTime;
            ElapsedSeconds = difference.TotalSeconds;
        }
    }
}
