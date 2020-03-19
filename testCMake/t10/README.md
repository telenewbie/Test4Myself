# 还缺少一个条件：

如果需要查找OpenSSL的时候，因为是Android编译的，所以需要修改`/usr/share/cmake-3.7/Module/FindOpenSSL.cmak` 里面对于Find_* 需要增加参数`NO_CMAKE_FIND_ROOT_PATH`
