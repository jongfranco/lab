// Copyright (C) 2016 Google Inc.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
////////////////////////////////////////////////////////////////////////////////

#include <stddef.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <utility>

#include "deepmind/engine/context.h"
#include "deepmind/engine/lua_maze_generation.h"
#include "deepmind/engine/lua_random.h"
#include "deepmind/engine/lua_text_level_maker.h"
#include "deepmind/include/deepmind_context.h"
#include "deepmind/level_generation/text_level/lua_bindings.h"
#include "deepmind/lua/vm.h"
#include "deepmind/tensor/lua_tensor.h"

namespace lua = deepmind::lab::lua;
namespace tensor = deepmind::lab::tensor;
using deepmind::lab::Context;
using deepmind::lab::LuaMazeGeneration;
using deepmind::lab::LuaRandom;
using deepmind::lab::LuaSnippetEmitter;
using deepmind::lab::LuaTextLevelMaker;

extern "C" {

int dmlab_create_context(const char* runfiles_path, DeepmindContext* ctx,
                         bool (*file_reader_override)(const char* file_name,
                                                      char** buff,
                                                      size_t* size)) {
  lua::Vm lua_vm = lua::CreateVm();
  lua_State* L = lua_vm.get();
  tensor::LuaTensorRegister(L);
  LuaMazeGeneration::Register(L);
  LuaRandom::Register(L);
  LuaTextLevelMaker::Register(L);
  LuaSnippetEmitter::Register(L);

  ctx->userdata = new Context(std::move(lua_vm), runfiles_path, &ctx->calls,
                              &ctx->hooks, file_reader_override);
  return 0;
}

void dmlab_release_context(DeepmindContext* ctx) {
  delete static_cast<Context*>(ctx->userdata);
}

}  // extern "C"
