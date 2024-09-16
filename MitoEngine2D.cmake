add_executable("MitoEngine2D"
  "src/AssetStore/AssetManager.cpp"
  "src/AssetStore/AssetManager.h"
  "src/AssetStore/RendererManager.cpp"
  "src/AssetStore/RendererManager.h"
  "src/Audio/AudioManager.cpp"
  "src/Audio/AudioManager.h"
  "src/Events/CollisionEnterEvent.h"
  "src/Events/CollisionExitEvent.h"
  "src/Events/CollisionStayEvent.h"
  "src/Events/Event.h"
  "src/Events/EventBus.h"
  "src/Events/GameObjectCreatedEvent.h"
  "src/Events/GlobalEventBus.cpp"
  "src/Events/GlobalEventBus.h"
  "src/Events/KeyPressedEvent.h"
  "src/Events/KeyReleasedEvent.h"
  "src/Events/MouseButtonPressedEvent.h"
  "src/Events/MouseInteractedEvent.cpp"
  "src/Events/MouseInteractedEvent.h"
  "src/Events/OpenLevelEvent.h"
  "src/Game/Game.cpp"
  "src/Game/Game.h"
  "src/GameObjects/Component.cpp"
  "src/GameObjects/Component.h"
  "src/GameObjects/GameObject.cpp"
  "src/GameObjects/GameObject.h"
    "src/GameObjects/Systems/CollisionSystem.cpp"
    "src/GameObjects/Systems/CollisionSystem.h"
    "src/GameObjects/Systems/RenderSystem.cpp"
    "src/GameObjects/Systems/RenderSystem.h"
  "src/Logger/Logger.cpp"
  "src/Logger/Logger.h"
  "src/LuaScripting/LuaScript.cpp"
  "src/LuaScripting/LuaScript.h"
    "src/eventbus/detail/function_traits.hpp"
  "src/eventbus/event_bus.hpp"
  "src/imgui/imconfig.h"
  "src/imgui/imgui.cpp"
  "src/imgui/imgui.h"
  "src/imgui/imgui_demo.cpp"
  "src/imgui/imgui_draw.cpp"
  "src/imgui/imgui_impl_sdl2.cpp"
  "src/imgui/imgui_impl_sdl2.h"
  "src/imgui/imgui_impl_sdlrenderer2.cpp"
  "src/imgui/imgui_impl_sdlrenderer2.h"
  "src/imgui/imgui_internal.h"
  "src/imgui/imgui_tables.cpp"
  "src/imgui/imgui_widgets.cpp"
  "src/imgui/imstb_rectpack.h"
  "src/imgui/imstb_textedit.h"
  "src/imgui/imstb_truetype.h"
"src/main.cpp"
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("MitoEngine2D" PROPERTIES
    OUTPUT_NAME "MitoEngine2D"
    ARCHIVE_OUTPUT_DIRECTORY "bin/windows-Debug/x86_64"
    LIBRARY_OUTPUT_DIRECTORY "bin/windows-Debug/x86_64"
    RUNTIME_OUTPUT_DIRECTORY "bin/windows-Debug/x86_64"
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Debug) # Include dirs
  target_include_directories("MitoEngine2D" PRIVATE
    libs/SDL2/include
    libs/SDL2_image/include
    libs/SDL2_ttf/include
    libs/glm
    libs/lua
    libs/sol
  )
endif()
target_compile_definitions("MitoEngine2D" PRIVATE
  $<$<CONFIG:Debug>:DEBUG>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug) # Lib dirs
  target_link_directories("MitoEngine2D" PRIVATE
    libs/SDL2/lib/x64
    libs/SDL2_image/lib/x64
    libs/SDL2_ttf/lib/x64
    libs/lua
  )
endif()
target_link_libraries("MitoEngine2D"
  $<$<CONFIG:Debug>:SDL2main>
  $<$<CONFIG:Debug>:SDL2>
  $<$<CONFIG:Debug>:SDL2_image>
  $<$<CONFIG:Debug>:SDL2_ttf>
  $<$<CONFIG:Debug>:liblua53.a>
)
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  if (NOT MSVC)
  endif()
endif()
if (MSVC)
  target_compile_options("MitoEngine2D" PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:/MP>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:/MDd>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:/Z7>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:/MP>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:/MDd>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:/Z7>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:/EHsc>
  )
else()
  target_compile_options("MitoEngine2D" PRIVATE
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-m64>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:C>>:-g>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-m64>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-g>
    $<$<AND:$<CONFIG:Debug>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Debug)
  set_target_properties("MitoEngine2D" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
add_custom_command(TARGET MitoEngine2D POST_BUILD
  COMMAND IF EXIST libs/SDL2/lib/x64/SDL2.dll\ (xcopy /Q /E /Y /I libs/SDL2/lib/x64/SDL2.dll bin/windows-Debug/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/SDL2/lib/x64/SDL2.dll bin/windows-Debug/x86_64 > nul)
  COMMAND IF EXIST libs/SDL2_image/lib/x64/SDL2_image.dll\ (xcopy /Q /E /Y /I libs/SDL2_image/lib/x64/SDL2_image.dll bin/windows-Debug/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/SDL2_image/lib/x64/SDL2_image.dll bin/windows-Debug/x86_64 > nul)
  COMMAND IF EXIST libs/SDL2_ttf/lib/x64/SDL2_ttf.dll\ (xcopy /Q /E /Y /I libs/SDL2_ttf/lib/x64/SDL2_ttf.dll bin/windows-Debug/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/SDL2_ttf/lib/x64/SDL2_ttf.dll bin/windows-Debug/x86_64 > nul)
  COMMAND IF EXIST libs/lua/lua53.dll\ (xcopy /Q /E /Y /I libs/lua/lua53.dll bin/windows-Debug/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/lua/lua53.dll bin/windows-Debug/x86_64 > nul)
  COMMAND IF EXIST libs/lua/liblua53.a.lib\ (xcopy /Q /E /Y /I libs/lua/liblua53.a.lib bin/windows-Debug/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/lua/liblua53.a.lib bin/windows-Debug/x86_64 > nul)
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("MitoEngine2D" PROPERTIES
    OUTPUT_NAME "MitoEngine2D"
    ARCHIVE_OUTPUT_DIRECTORY "bin/windows-Release/x86_64"
    LIBRARY_OUTPUT_DIRECTORY "bin/windows-Release/x86_64"
    RUNTIME_OUTPUT_DIRECTORY "bin/windows-Release/x86_64"
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release) # Include dirs
  target_include_directories("MitoEngine2D" PRIVATE
    libs/SDL2/include
    libs/SDL2_image/include
    libs/SDL2_ttf/include
    libs/glm
    libs/lua
    libs/sol
  )
endif()
target_compile_definitions("MitoEngine2D" PRIVATE
  $<$<CONFIG:Release>:NDEBUG>
)
if(CMAKE_BUILD_TYPE STREQUAL Release) # Lib dirs
  target_link_directories("MitoEngine2D" PRIVATE
    libs/SDL2/lib/x64
    libs/SDL2_image/lib/x64
    libs/SDL2_ttf/lib/x64
    libs/lua
  )
endif()
target_link_libraries("MitoEngine2D"
  $<$<CONFIG:Release>:SDL2main>
  $<$<CONFIG:Release>:SDL2>
  $<$<CONFIG:Release>:SDL2_image>
  $<$<CONFIG:Release>:SDL2_ttf>
  $<$<CONFIG:Release>:liblua53.a>
)
if(CMAKE_BUILD_TYPE STREQUAL Release)
  if (NOT MSVC)
  endif()
endif()
if (MSVC)
  target_compile_options("MitoEngine2D" PRIVATE
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:/MP>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:/Ot>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:/MD>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:/MP>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:/Ot>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:/MD>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:/EHsc>
  )
else()
  target_compile_options("MitoEngine2D" PRIVATE
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-m64>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:C>>:-O2>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-m64>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-O2>
    $<$<AND:$<CONFIG:Release>,$<COMPILE_LANGUAGE:CXX>>:-std=c++17>
  )
endif()
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set_target_properties("MitoEngine2D" PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE False
    INTERPROCEDURAL_OPTIMIZATION False
  )
endif()
add_custom_command(TARGET MitoEngine2D POST_BUILD
  COMMAND IF EXIST libs/SDL2/lib/x64/SDL2.dll\ (xcopy /Q /E /Y /I libs/SDL2/lib/x64/SDL2.dll bin/windows-Release/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/SDL2/lib/x64/SDL2.dll bin/windows-Release/x86_64 > nul)
  COMMAND IF EXIST libs/SDL2_image/lib/x64/SDL2_image.dll\ (xcopy /Q /E /Y /I libs/SDL2_image/lib/x64/SDL2_image.dll bin/windows-Release/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/SDL2_image/lib/x64/SDL2_image.dll bin/windows-Release/x86_64 > nul)
  COMMAND IF EXIST libs/SDL2_ttf/lib/x64/SDL2_ttf.dll\ (xcopy /Q /E /Y /I libs/SDL2_ttf/lib/x64/SDL2_ttf.dll bin/windows-Release/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/SDL2_ttf/lib/x64/SDL2_ttf.dll bin/windows-Release/x86_64 > nul)
  COMMAND IF EXIST libs/lua/lua53.dll\ (xcopy /Q /E /Y /I libs/lua/lua53.dll bin/windows-Release/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/lua/lua53.dll bin/windows-Release/x86_64 > nul)
  COMMAND IF EXIST libs/lua/liblua53.a.lib\ (xcopy /Q /E /Y /I libs/lua/liblua53.a.lib bin/windows-Release/x86_64 > nul) ELSE (xcopy /Q /Y /I libs/lua/liblua53.a.lib bin/windows-Release/x86_64 > nul)
)