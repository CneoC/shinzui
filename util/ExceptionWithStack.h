//#
//# Exception with Stack
//# $Revision: 1.1 $
//# Copyright 2005 by Eric Y. Theriault
//# All Rights Reserved.
//# http://www.eyt.ca/
//#
#ifndef _EYT_EXCEPTIONWITHSTACK_H
#define _EYT_EXCEPTIONWITHSTACK_H

//#
//# Required Includes
//#
#ifndef STD_INCLUDE_EXCEPTION
#include <exception>
#define STD_INCLUDE_EXCEPTION
#endif
#ifndef STD_INCLUDE_STRING
#include <string>
#define STD_INCLUDE_STRING
#endif
#ifdef __linux__
#ifndef STD_INCLUDE_VECTOR
#include <vector>
#define STD_INCLUDE_VECTOR
#endif
#endif

//
// <summary>Exception with the Stack Trace</summary>
//
// This class implements a simple Exception that takes an English warning and
// the current location of the failure.
//
// This class is inspired from http://www-128.ibm.com/developerworks/linux/library/l-cppexcep.html?ca=dgr-lnxw09ExceptionTricks .
//
class ExceptionWithStack : public std::exception {
private:
    //
    // Filename where the exception was thrown from.
    //
    const char *filename_;

    //
    // Line where the exception was thrown from.
    //
    const int line_;

    //
    // Explanation of the failure.
    //
    std::string reason_;

#ifdef __linux__
    //
    // Stack Trace
    //
    std::vector<std::string> stack_;
#endif

public:
    //
    // Default Constructor
    //
    ExceptionWithStack(
                       const char *filename,
                       int line,
                       const char *reason
                      );

    //
    // Copy Constructor and assignment opreator.
    //
    // <group>
    ExceptionWithStack( const ExceptionWithStack & ex );
    ExceptionWithStack & operator= ( const ExceptionWithStack & );
    // </group>

    //
    // Destructor
    //
    virtual ~ExceptionWithStack() throw();

    //
    // Acquire the filename where the exception was raised from.
    //
    const char *getFilename() const { return filename_; }

    //
    // Acquire the line number where the exception was raised from.
    //
    int getLineNumber() const { return line_; }

    //
    // Explanation of the failure.
    //
    virtual const char * what() const throw();

    //
    // Display the stack trace on Standard Output.
    //
    void printStackTrace() const;
};

#define MY_THROW(str)    throw ExceptionWithStack( __FILE__, __LINE__, str );

#endif
