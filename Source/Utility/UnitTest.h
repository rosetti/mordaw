/*
  ==============================================================================

    UnitTest.h
    Created: 31 Aug 2015 7:32:08pm
    Author:  dtl

  ==============================================================================
*/

#ifndef UNITTEST_H_INCLUDED
#define UNITTEST_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

struct UnitTestClasses
{
    class RegionTest;
    class UnitTestsComponent;
    class TestRunnerThread;
    
    //==============================================================================
    // This subclass of UnitTestRunner is used to redirect the test output to our
    // TextBox, and to interrupt the running tests when our thread is asked to stop..
    class TestRunner  : public UnitTestRunner
    {
    public:
        TestRunner (TestRunnerThread& trt)  : owner (trt)
        {
        }
        
        void logMessage (const String& message) override
        {
            owner.logMessage (message);
        }
        
        bool shouldAbortTests() override
        {
            return owner.threadShouldExit();
        }
        
    private:
        TestRunnerThread& owner;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestRunner);
    };
    
    //==============================================================================
    class TestRunnerThread  : public Thread,
    private Timer
    {
    public:
        TestRunnerThread (UnitTestsComponent& utd)
        : Thread ("Unit Tests"),
        owner (utd)
        {
        }
        
        void run() override
        {
            TestRunner runner (*this);
            runner.runAllTests();
            
            startTimer (50); // when finished, start the timer which will
            // wait for the thread to end, then tell our component.
        }
        
        void logMessage (const String& message)
        {
            MessageManagerLock mm (this);
            
            if (mm.lockWasGained()) // this lock may fail if this thread has been told to stop
                owner.logMessage (message);
        }
        
        void timerCallback() override
        {
            if (! isThreadRunning())
                owner.testFinished(); // inform the demo page when done, so it can delete this thread.
        }
        
    private:
        UnitTestsComponent& owner;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TestRunnerThread);
    };
    
    
    //==============================================================================
    class UnitTestsComponent : public Component,
                               public Button::Listener
    {
    public:
        UnitTestsComponent()
        : startTestButton ("Run Unit Tests...")
        {
            setOpaque (true);
            
            addAndMakeVisible (startTestButton);
            addAndMakeVisible (testResultsBox);
            testResultsBox.setMultiLine (true);
            testResultsBox.setFont (Font (Font::getDefaultMonospacedFontName(), 12.0f, Font::plain));
            
            startTestButton.addListener (this);
            
            logMessage ("This panel runs all the built-in JUCE unit-tests.\n");
            logMessage ("To add your own unit-tests, see the JUCE_UNIT_TESTS macro.");
        }
        
        ~UnitTestsComponent()
        {
            stopTest();
        }
        
        //==============================================================================
        void paint (Graphics& g) override
        {
            g.fillAll (Colours::grey);
        }
        
        void resized() override
        {
            startTestButton.setBounds(20, 20, 30, 20);
            startTestButton.setAlwaysOnTop(true);
            testResultsBox.setBounds (60, 50, 100, 100);
            testResultsBox.setAlwaysOnTop(true);
        }
        
        void buttonClicked (Button* buttonThatWasClicked) override
        {
            if (buttonThatWasClicked == &startTestButton)
            {
                startTest();
            }
        }
        
        void startTest()
        {
            testResultsBox.clear();
            startTestButton.setEnabled (false);
            
            currentTestThread = new TestRunnerThread (*this);
            currentTestThread->startThread();
        }
        
        void stopTest()
        {
            if (currentTestThread != nullptr)
            {
                currentTestThread->stopThread (15000);
                currentTestThread = nullptr;
            }
        }
        
        void logMessage (const String& message)
        {
            testResultsBox.moveCaretToEnd();
            testResultsBox.insertTextAtCaret (message + newLine);
            testResultsBox.moveCaretToEnd();
        }
        
        void testFinished()
        {
            stopTest();
            startTestButton.setEnabled (true);
            logMessage (newLine + "*** Tests finished ***");
        }
        
    private:
        ScopedPointer<TestRunnerThread> currentTestThread;
        
        TextButton startTestButton;
        TextEditor testResultsBox;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UnitTestsComponent);
    };
};


#endif  // UNITTEST_H_INCLUDED
