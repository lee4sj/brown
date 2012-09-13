#ifndef TA_TESTER_H
#define TA_TESTER_H

#include <QMap>
#include <ostream>
#include <QString>

class Camera;
class CamtransCamera;

struct TestResult
{
    QString message;
    bool passed;
};


/*!
    @class  TestResults
    @brief  Collection of test case results.
*/
class TestResults : public QMap<QString, TestResult>
{
public:
    bool allPassed();       //!< True if all test cases passed
    void appendAll(const TestResults &items);
    void toOstream(std::ostream &output);
};

/*!
    @class  TA_Tester
    @brief  Framework for testing student code.
    @author Ben Herila

    Note that CS123Matrix gets linked in.
*/
class TA_Tester
{
public:
    TA_Tester();
    virtual ~TA_Tester();

    CamtransCamera *camtrans;       //!< Pointer to instance of Camtrans (perspective) camera.
    double          epsilon;        //!< How close do we need to be? Default 0.00001.
    unsigned        testIterations; //!< Number of test iterations to run. Default 10.

    TestResults     test_camtrans();   //!< Tests Camtrans functionality
    TestResults     test_matrix4x4();  //!< Tests CS123Matrix.cpp functionality
    TestResults     test_all();        //!< Tests all functionality

};


#endif // TA_TESTER_H
