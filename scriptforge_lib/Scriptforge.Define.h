#pragma once
#ifndef SCRIPTFORGE_DEFINE_H
#define SCRIPTFORGE_DEFINE_H
#define SCRIPTFORGE_MODULE
#if defined(SCRIPTFORGE_MODULE)
#define _SCRIPTFORGE_API_ export
#define SCRIPTFORGE_IMPORT(x) import x;
#define SCRIPTFORGE_IMPORT_H(x) SCRIPTFORGE_IMPORT(x)
#define SCRIPTFORGE_IMPORT_HPP(x) SCRIPTFORGE_IMPORT(x)

#elif defined(SCRIPTFORGE_HEAD)
#ifdef BUILD_STATIC
#define _SCRIPTFORGE_API_
#else
#if defined(_WIN32) || defined(_WIN64)
#define _SCRIPTFORGE_API_ __declspec(dllexport)
#endif
#endif
#define SCRIPTFORGE_IMPORT(x) #include "x.ixx"
#define SCRIPTFORGE_IMPORT_H(x) #include "x.h"
#define SCRIPTFORGE_IMPORT_HPP(x) #include "x.hpp"
#endif
#endif