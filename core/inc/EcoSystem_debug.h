#pragma once

/// USER_SECTION_START 1

/// USER_SECTION_END

// Debugging
#ifdef NDEBUG
#define ES_CONSOLE(msg)
#define ES_CONSOLE_FUNCTION(msg)
#else
#include <iostream>

#define ES_DEBUG
#define ES_CONSOLE_STREAM std::cout

#define ES_CONSOLE(msg) ES_CONSOLE_STREAM << msg;
#define ES_CONSOLE_FUNCTION(msg) ES_CONSOLE_STREAM << __PRETTY_FUNCTION__ << " " << msg;
#endif

/// USER_SECTION_START 2

/// USER_SECTION_END

#ifdef ES_PROFILING
#include "easy/profiler.h"
#include <easy/arbitrary_value.h> // EASY_VALUE, EASY_ARRAY are defined here

#define ES_PROFILING_BLOCK_C(text, color) EASY_BLOCK(text, color)
#define ES_PROFILING_NONSCOPED_BLOCK_C(text, color) EASY_NONSCOPED_BLOCK(text, color)
#define ES_PROFILING_END_BLOCK EASY_END_BLOCK
#define ES_PROFILING_FUNCTION_C(color) EASY_FUNCTION(color)
#define ES_PROFILING_BLOCK(text, colorStage) ES_PROFILING_BLOCK_C(text,profiler::colors::  colorStage)
#define ES_PROFILING_NONSCOPED_BLOCK(text, colorStage) ES_PROFILING_NONSCOPED_BLOCK_C(text,profiler::colors::  colorStage)
#define ES_PROFILING_FUNCTION(colorStage) ES_PROFILING_FUNCTION_C(profiler::colors:: colorStage)
#define ES_PROFILING_THREAD(name) EASY_THREAD(name)

#define ES_PROFILING_VALUE(name, value) EASY_VALUE(name, value)
#define ES_PROFILING_TEXT(name, value) EASY_TEXT(name, value)

#else
#define ES_PROFILING_BLOCK_C(text, color)
#define ES_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define ES_PROFILING_END_BLOCK
#define ES_PROFILING_FUNCTION_C(color)
#define ES_PROFILING_BLOCK(text, colorStage)
#define ES_PROFILING_NONSCOPED_BLOCK(text, colorStage)
#define ES_PROFILING_FUNCTION(colorStage)
#define ES_PROFILING_THREAD(name)

#define ES_PROFILING_VALUE(name, value)
#define ES_PROFILING_TEXT(name, value)
#endif

// Special expantion tecniques are required to combine the color name
#define CONCAT_SYMBOLS_IMPL(x, y) x##y
#define CONCAT_SYMBOLS(x, y) CONCAT_SYMBOLS_IMPL(x, y)



// Different color stages
#define ES_COLOR_STAGE_1 50
#define ES_COLOR_STAGE_2 100
#define ES_COLOR_STAGE_3 200
#define ES_COLOR_STAGE_4 300
#define ES_COLOR_STAGE_5 400
#define ES_COLOR_STAGE_6 500
#define ES_COLOR_STAGE_7 600
#define ES_COLOR_STAGE_8 700
#define ES_COLOR_STAGE_9 800
#define ES_COLOR_STAGE_10 900
#define ES_COLOR_STAGE_11 A100 
#define ES_COLOR_STAGE_12 A200 
#define ES_COLOR_STAGE_13 A400 
#define ES_COLOR_STAGE_14 A700 


// General
#define ES_GENERAL_PROFILING_COLORBASE Cyan
#define ES_GENERAL_PROFILING_BLOCK_C(text, color) ES_PROFILING_BLOCK_C(text, color)
#define ES_GENERAL_PROFILING_NONSCOPED_BLOCK_C(text, color) ES_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define ES_GENERAL_PROFILING_END_BLOCK ES_PROFILING_END_BLOCK;
#define ES_GENERAL_PROFILING_FUNCTION_C(color) ES_PROFILING_FUNCTION_C(color)
#define ES_GENERAL_PROFILING_BLOCK(text, colorStage) ES_PROFILING_BLOCK(text, CONCAT_SYMBOLS(ES_GENERAL_PROFILING_COLORBASE, colorStage))
#define ES_GENERAL_PROFILING_NONSCOPED_BLOCK(text, colorStage) ES_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(ES_GENERAL_PROFILING_COLORBASE, colorStage))
#define ES_GENERAL_PROFILING_FUNCTION(colorStage) ES_PROFILING_FUNCTION(CONCAT_SYMBOLS(ES_GENERAL_PROFILING_COLORBASE, colorStage))
#define ES_GENERAL_PROFILING_VALUE(name, value) ES_PROFILING_VALUE(name, value)
#define ES_GENERAL_PROFILING_TEXT(name, value) ES_PROFILING_TEXT(name, value)


/// USER_SECTION_START 3

/// USER_SECTION_END
