VersionOne TestComplete 8 Plugin
================================

The plugin enables users to post test run results to VersionOne server. The workflow is similar to ServiceHost integration: test status updates depending on Reference value, TestRun and Defect creation, calculation of test run statistics.

Source code can be built on a machine with the following software (tested on Win XP SP3 and Win 7): 
- VS 2008 (including C++), .NET Framework 3.5
- TestComplete 8
- TestComplete 8 SDK

Stdafx.h file in VersionOne.TestComplete.Plugin should be modified first to contain correct paths to TestComplete components in #import statements.

To install and configure the plugin, user should perform the following steps (assuming that TestComplete is installed, and there are Project Suites and Projects that should be used to provide data to VersionOne server):

0. Configure VersionOne*
1. Copy binary package contents to Bin folder of TestComplete installation, usually C:\Program Files\Automated QA\TestComplete 8\Bin.
2. Edit App.config file - you can set up VersionOne path, credentials, integration behavior settings there.*
3. In TestComplete app, choose File > Install extension... > Add... and select VersionOne.TestComplete.Plugin.pls copied in step 1.
4. As soon as plugin is installed, VersionOne Integration project item is available in TestComplete. It should be added to each and every project that is intended to post data to VersionOne server.

* ServiceHost TestComplete integration documentation provides detailed explanation.

There is an issue that could lead to collisions in Version One test results in some cases. If Test Complete project suite contains several projects, during project run the plug-in posts extra test run information. These tests belong to other projects and in case of identical names matching VersionOne tests could be updated. 
Thus, currently we support only one project in Test Complete project suite.