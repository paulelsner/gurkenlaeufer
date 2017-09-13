# gurkenlaeufer
Simple cucumber like test runner.

[![Build Status](https://travis-ci.org/paulelsner/gurkenlaeufer.svg?branch=master)](https://travis-ci.org/paulelsner/gurkenlaeufer)

# What it does for you
The goal of this project is to provide a simple way to write cucumber tests for your C++ projects. Other projects like [cucumber-cpp](https://github.com/cucumber/cucumber-cpp) could already do that, but they add a huge amount of complexity for your tests. Like using ruby in your CI environment and handle running multiple processes.

Here are the benefits listed:
* no external dependencies beside c++11 compiler
* no ruby required
* single process
* parallel test execution
* simple debugging

# How this is achieved

Gurkenlaeufer has three parts:
1. Simple gherkin file parser
2. Test step framework
3. Test runner

The gherkin file parser takes lines as a input usually read from a feature files. Here it is up to the user where the line come from. Take a look into the example for a simple feature file reader.

The test step framework helps you defining you test steps. With a regex you can match steps and with captured you get access to variable parameters.

The test runner uses the [google-test](https://github.com/google/googletest) framework. From the parsed test cases multiple google tests will be created. This enables you using all the test features google-test gives you for you cucumber tests. Like running tests in parallel and generating junit output.

# Supported Features
* Scenarios
* Background
* Scenario Outline with examples

# Planes Features
* Before and After hooks
