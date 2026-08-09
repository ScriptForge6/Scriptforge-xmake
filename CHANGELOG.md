# Scriptforge Library Changelog

**Other Language:**[Chinese](CHANGELOG-CN.md)

# v0.1.2
Updated version system to v0.1.2 in [Scriptforge.Tree.ixx](ScriptforgeLib/Scriptforge.Tree.ixx) and [Scriptforge.ixx](ScriptforgeLib/Scriptforge.ixx)

# v0.1.1
Added [Scriptforge.AntiDebug.ixx](ScriptforgeLib/Scriptforge.AntiDebug.ixx), which implements anti-debugging features.

# v0.1.0
All implementation files have been removed and merged into the module file, thereby eliminating the need for explicit template instantiation.

# v0.0.1
added version system(files:[Scriptforge.Version.cpp](ScriptforgeLib/Scriptforge.Version.cpp),[Scriptforge.Version.ixx](ScriptforgeLib/Scriptforge.Version.ixx)) and itself version in [Scriptforge.ixx](ScriptforgeLib/Scriptforge.ixx)

# v0.0.0(before v0.0.1)
added initial project structure and basic modules:
```
Scriptforge/
├── Scriptforge.sln
├── README.md
├──README-CN.md
├── ScriptforgeLib/
│    ├── Scriptforge.ixx
│    ├── Scriptforge.BitPack.ixx
│    ├── Scriptforge.Err.ixx
│    ├── Scriptforge.Log.ixx
│    ├── Scriptforge.Tree.ixx
│    ├── Scriptforge.BitPack.cpp
│    ├── Scriptforge.Err.cpp
│    ├── Scriptforge.Log.cpp
│    └──Scriptforge.Tree.cpp
└── ScriptforgeTest/
        └── ScriptforgeTest.cpp
```