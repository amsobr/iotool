//
// Created by to on 17/02/22.
//

/**
 * Forward declarations for the YES library
 */

#pragma once

#include <memory>

#define DECL_PTR(T) typedef std::shared_ptr<T> T##Ptr;

#define DECL_CONST_PTR(T) typedef std::shared_ptr<T> T##ConstPtr;

namespace rps
{

class Context;
DECL_PTR(Context)
DECL_CONST_PTR(Context)

class Command;
DECL_PTR(Command)
DECL_CONST_PTR(Command)

class AbstractCommand;
DECL_PTR(AbstractCommand)
DECL_CONST_PTR(AbstractCommand)

class Script;
DECL_PTR(Script)
DECL_CONST_PTR(Script)

// Decoders are only moved around as cont's after being built.
class Decoder;
//DECL_PTR(Decoder)
DECL_CONST_PTR(Decoder)

class CommandTree;
DECL_PTR(CommandTree)
DECL_CONST_PTR(CommandTree)

}
