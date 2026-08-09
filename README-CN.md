<center>

  <img src="src/ScriptForge.png" alt="Scriptforge" width="100"/>
  
  <h1>Scriptforge</h1>
  
  <h2>一个有着许许多多奇奇怪怪功能的开源库</h2>

[README主版本(English)](README.md)

[更新日志](CHANGELOG-CN.md)

</center>

# ⚠️ 维护通知

本库目前正在进行重大重构和更新。
请注意：

- **编译错误**：当前版本可能会出现编译错误
- **API稳定性**：在此期间无法保证API稳定性
- **部分功能**：可能暂时不可用

我们建议在更新完成前使用最新的稳定版本。


### 🎉 项目重新开始维护！

---

<!-- TREE_START -->
```
Scriptforge
├── CHANGELOG-CN.md
├── CHANGELOG.md
├── GenerateRandom
│   ├── GenerateRandom.vcxproj
│   ├── GenerateRandom.vcxproj.filters
│   └── main.cpp
├── LICENSE-THIRD-PARTY.txt
├── LICENSE.txt
├── README-CN.md
├── README.md
├── Scriptforge.sln
├── ScriptforgeLib
│   ├── Doxyfile
│   ├── ScriptForge.ErrCode.ixx
│   ├── Scriptforge.AntiDebug.RandomDefine.ixx
│   ├── Scriptforge.AntiDebug.cpp
│   ├── Scriptforge.AntiDebug.ixx
│   ├── Scriptforge.Argv.ixx
│   ├── Scriptforge.Err.ixx
│   ├── Scriptforge.ErrCode.throwError.ixx
│   ├── Scriptforge.LanguageCode.ixx
│   ├── Scriptforge.Local.ixx
│   ├── Scriptforge.Log.ixx
│   ├── Scriptforge.Msg.ixx
│   ├── Scriptforge.Pch.ixx
│   ├── Scriptforge.ProcessPool.ixx
│   ├── Scriptforge.RingBuffer.ixx
│   ├── Scriptforge.StringConversion.ixx
│   ├── Scriptforge.ThreadError.ixx
│   ├── Scriptforge.Tree.ixx
│   ├── Scriptforge.Version.ixx
│   ├── Scriptforge.ixx
│   ├── ScriptforgeLib.vcxproj
│   ├── ScriptforgeLib.vcxproj.filters
│   ├── lang
│   │   ├── en.json
│   │   └── zh.json
│   └── 统计代码行数.ps1
├── ScriptforgeTest
│   ├── AntiDebug.Test.ixx
│   ├── Err.Test.ixx
│   ├── ErrCode.Test.ixx
│   ├── ErrCode.cpp
│   ├── Local.Test.ixx
│   ├── Msg.Test.ixx
│   ├── ScriptforgeTest.vcxproj
│   ├── ScriptforgeTest.vcxproj.filters
│   ├── StringConversion.Test.ixx
│   ├── Test.cpp
│   ├── ThreadError.Test.ixx
│   └── lang
│       ├── en.json
│       └── zh.json
├── repo_tree.md
├── src
│   └── ScriptForge.png
├── thirdParty
│   ├── googletest-v17.0
│   │   ├── googlemock
│   │   │   └── include
│   │   │       └── gmock
│   │   │           ├── gmock-actions.h
│   │   │           ├── gmock-cardinalities.h
│   │   │           ├── gmock-function-mocker.h
│   │   │           ├── gmock-matchers.h
│   │   │           ├── gmock-more-actions.h
│   │   │           ├── gmock-more-matchers.h
│   │   │           ├── gmock-nice-strict.h
│   │   │           ├── gmock-spec-builders.h
│   │   │           ├── gmock.h
│   │   │           └── internal
│   │   │               ├── custom
│   │   │               │   ├── README.md
│   │   │               │   ├── gmock-generated-actions.h
│   │   │               │   ├── gmock-matchers.h
│   │   │               │   └── gmock-port.h
│   │   │               ├── gmock-internal-utils.h
│   │   │               ├── gmock-port.h
│   │   │               └── gmock-pp.h
│   │   ├── googletest
│   │   │   └── include
│   │   │       └── gtest
│   │   │           ├── gtest-assertion-result.h
│   │   │           ├── gtest-death-test.h
│   │   │           ├── gtest-matchers.h
│   │   │           ├── gtest-message.h
│   │   │           ├── gtest-param-test.h
│   │   │           ├── gtest-printers.h
│   │   │           ├── gtest-spi.h
│   │   │           ├── gtest-test-part.h
│   │   │           ├── gtest-typed-test.h
│   │   │           ├── gtest.h
│   │   │           ├── gtest_pred_impl.h
│   │   │           ├── gtest_prod.h
│   │   │           └── internal
│   │   │               ├── custom
│   │   │               │   ├── README.md
│   │   │               │   ├── gtest-port.h
│   │   │               │   ├── gtest-printers.h
│   │   │               │   └── gtest.h
│   │   │               ├── gtest-death-test-internal.h
│   │   │               ├── gtest-filepath.h
│   │   │               ├── gtest-internal.h
│   │   │               ├── gtest-param-util.h
│   │   │               ├── gtest-port-arch.h
│   │   │               ├── gtest-port.h
│   │   │               ├── gtest-string.h
│   │   │               └── gtest-type-util.h
│   │   └── lib
│   │       ├── gmock.lib
│   │       ├── gmock_main.lib
│   │       ├── gtest.lib
│   │       └── gtest_main.lib
│   ├── json
│   │   ├── json.cppm
│   │   └── nlohmann
│   │       ├── json.hpp
│   │       └── json_fwd.hpp
│   └── utf8
│       ├── utf8
│       │   ├── checked.h
│       │   ├── core.h
│       │   ├── cpp11.h
│       │   ├── cpp17.h
│       │   ├── cpp20.h
│       │   └── unchecked.h
│       └── utf8.h
└── 更新方向.md
```

<!-- TREE_END -->

## 📝 使用说明
- 目前使用 Visual Studio sln 项目开发
- 如需 CMake 构建，请自行配置 CMakeLists.txt 文件
- 更多构建方式正在规划中...

 