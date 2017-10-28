# language: en
Feature: Addition
  In order to avoid silly mistakes
  As a math idiot 
  I want to be told the sum of two numbers

  Background:
    Then nostep 

  @Print
  Scenario: Add two numbers.
    Given I have entered 1 into the calculator
    And I have entered 1 into the calculator
    When I press add
    Then the result should be 2 on the screen

  @Echo
  Scenario Outline: Add two numbers.
    Given I have entered <input_1> into the calculator
    And I have entered <input 2> into the calculator
    When I press <button>
    Then the result should be <output> on the screen

  Examples:
    | input_1 | input 2 | button | output |
    | 20      | 30      | add    | 50     |
    | 2       | 5       | add    | 7      |
    | 0       | 40      | add    | 40     |

  # this scenario should not run. It will fail
  @Ignore
  Scenario: Do not run this
    Given I have entered 1 into the calculator
    And I have entered 1 into the calculator
    When I press add
    Then the result should be 3 on the screen

  @Echo @Print
  # A comment should be possible here
  Scenario Outline: add(+) two numbers with comments and two tags and special° characters\n\a *+'!:._,()/\§$ %= `´ <> @
    # A comment should be possible here
    Given I have entered <input_1> into the calculator
    And I have entered <input 2> into the calculator # A comment should be possible here
    # A comment should be possible here
    When I press <button>
    Then the result should be <output> on the screen
    But nobody should know
      """
      This is additional
      data
      to the step
      42!
      """

  Examples:
    | input_1 | input 2 | button | output |
    | 2       | 5       | add    | 7      |
    # A comment should be possible here
    | 0       | 40      | add    | 40     |
