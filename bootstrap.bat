winget install --accept-source-agreements --verbose --id Microsoft.VisualStudio.2022.Community ^
--override "--passive --wait --add Microsoft.VisualStudio.Workload.NativeDesktop;includeRecommended --add Microsoft.VisualStudio.Component.VC.ASAN --add Microsoft.VisualStudio.Component.VC.Llvm.Clang --add Microsoft.VisualStudio.Component.VC.Llvm.ClangToolset --add Microsoft.VisualStudio.ComponentGroup.NativeDesktop.Llvm.Clang"

winget install Ninja-build.Ninja Python.Python.3.12 Cppcheck.Cppcheck

cmd /K "C:\\Program^ Files\\Microsoft^ Visual^ Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat && vcpkg install" 

