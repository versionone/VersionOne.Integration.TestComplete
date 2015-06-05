using System;
using System.Collections.Generic;

namespace VersionOne.TestComplete.V1Connector {
    public class SuiteResult : AbstractResult {
        public const string Description = "TestComplete Suite";
        
        public string Name { get; set; }
        public int PassedTestCount { get; set; }
        public int FailedTestCount { get; set; }
        public int NotRunTestCount { get; set; }

        public readonly IList<TestResult> TestResults = new List<TestResult>();

        public SuiteResult() {
            PassedTestCount = 0;
            FailedTestCount = 0;
            NotRunTestCount = 0;
        }

        public void CalculateStatistics() {
            RecordEndTime();

            foreach(TestResult test in TestResults) {
                switch (test.State) {
                    case TestResult.TestRunState.Passed:
                        PassedTestCount++;
                        break;

                    case TestResult.TestRunState.Failed:
                        FailedTestCount++;
                        break;
                }
            }
        }
    }
}