//#
//# Exception with Stack
//# $Revision: 1.1 $
//# Copyright 2005 by Eric Y. Theriault
//# All Rights Reserved.
//# http://www.eyt.ca/
//#
#include "ExceptionWithStack.h"
#include <iostream>
#ifdef __linux__
#include <execinfo.h>
#endif

//
// Constructor
//
ExceptionWithStack::ExceptionWithStack(
                                       const char *filename,
                                       int line,
                                       const char *reason
                                      ):
    std::exception(),
    filename_( filename ),
    line_( line ),
    reason_( reason )
#ifdef __linux__
    , stack_()
#endif
{
#ifdef __linux__
    // Acquire the stack trace...
    void * array[25];
    size_t entries = backtrace( array, sizeof( array ) / sizeof( void* ) );
    char ** symbols = backtrace_symbols( array, entries );
    if ( symbols == 0 ) {
       // Probable out of memory condition; we'll skip
       // over the full stack.
       entries = 0;
    }

    // Throw it into our Vector.  We are skipping the
    // top-most element, since this will be this
    // constructor, which is not particularly interesting.
    // We are also skipping the next line, because the
    // __FILE__ and __LINE__ have that location already.
    stack_.reserve( entries > 2 ? entries - 2 : 0 );
    for ( size_t i = 2; i < entries; i++ ) {
        stack_.push_back( symbols[i] );
    }

    // Free up the allocated memory.
    free( symbols );
#endif
}

//
// Constructor
//
ExceptionWithStack::ExceptionWithStack( const ExceptionWithStack & ex ):
    std::exception( ex ),
    filename_( ex.filename_ ),
    line_( ex.line_ ),
    reason_( ex.reason_ )
#ifdef __linux__
    , stack_( ex.stack_ )
#endif
{
}

//
// Assignment Operator
//
ExceptionWithStack & ExceptionWithStack::operator=( 
                                                   const ExceptionWithStack & ex
                                                  )
{
    if ( this == &ex ) {
        return *this;
    }

    // This is a little ugly; we are effectively destroying our object and
    // using the copy-constructor to reconstruct this object via placement-new.
    // This is done to avoid calling our super-class operator=.
    this->~ExceptionWithStack();
    new ( this ) ExceptionWithStack ( ex );
    return *this;
}

//
// Destructor
//
ExceptionWithStack::~ExceptionWithStack() throw()
{
}

//
// Acquires the reason for the failure.
//
const char *ExceptionWithStack::what() const throw()
{
    return reason_.c_str();
}

//
// Acquire the Stack Trace.
//
void ExceptionWithStack::printStackTrace() const
{
    std::cout << reason_ << std::endl;
    std::cout << "   at " << filename_ << ":" << line_ << std::endl;
#ifdef __linux__
    for ( int i = 0, size = stack_.size(); i < size; ++ i ) {
        std::cout << "   at " << stack_[i] << std::endl;
    }
#endif
}
