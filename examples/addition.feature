# language: en
Feature: Addition
  In order to avoid silly mistakes
  As a math idiot 
  I want to be told the sum of two numbers

  Background:
    Then nostep 

  @Print
  Scenario: Add two numbers
    Given I have entered 1 into the calculator
    And I have entered 1 into the calculator
    When I press add
    Then the result should be 2 on the screen

  @Echo
  Scenario Outline: Add two numbers
    Given I have entered <input_1> into the calculator
    And I have entered <input_2> into the calculator
    When I press <button>
    Then the result should be <output> on the screen

  Examples:
    | input_1 | input_2 | button | output |
    | 20      | 30      | add    | 50     |
    | 2       | 5       | add    | 7      |
    | 0       | 40      | add    | 40     |
    | 0       | 40      | add    | 41     |
