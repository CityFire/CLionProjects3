cmake_minimum_required(VERSION 3.23)
project (Demo)

set(CMAKE_CXX_STANDARD 14)

message(STATUS "start running cmake...")

find_package(Boost 1.78.0 COMPONENTS system filesystem REQUIRED)

if(Boost_FOUND)

    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")

    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")

    message(STATUS "Boost_VERSION: ${Boost_VERSION}")

    include_directories(${Boost_INCLUDE_DIRS})

endif()

#add_executable(Demo main.cpp TestA.cpp TestA.h TestMain.cpp Clock.cpp Clock.h StringA.cpp StringA.h CountedObject.cpp CountedObject.h FriendTestMain.cpp Television.cpp Television.h TeleController.cpp TeleController.h Complex.cpp Complex.h complexTestMain.cpp Integer.cpp Integer.h OperatorTestMain.cpp StringTestMain.cpp StringUtil.cpp StringUtil.h vectorTestMain.cpp mapTestMain.cpp CStackTestMain.cpp CPPStackTestMain.cpp InheritTestMain.cpp InheritMainTest.cpp MultiInheritTestMain.cpp VirtualBTableTestMain.cpp VirtualFunTestMain.cpp VirtualFunTableTestMain.cpp AbstractVirtualTestMain.cpp PureDestructFunTestMain.cpp DynamicTestMain.cpp CShape.cpp CShape.h DynamicBase.h ExceptionTestMain.cpp SingletonTestMain.cpp SingletonAutoPtrTestMain.cpp iostreamTestMain.cpp TemplateTestMain.cpp max.cpp max.h ClassTemplateTestMain.cpp Stack.h Stack2.h Stack3.h SingletonTemplateTestMain.cpp SingletonTemplate.h STLTestMain.cpp STLFunObjectTestMain.cpp STLAlogrithmTestMain.cpp STLAdapterTestMain.cpp STLIteratorAdapterTestMain.cpp STLContainerAdapterTestMain.cpp STLBootPointerTestMain.cpp ThreadTestMain.cpp)

#if(Boost_FOUND)

    #target_link_libraries(Demo ${Boost_LIBRARIES})

#endif()

set(BOOST_ROOT "/usr/local/Cellar/boost/1.78.0_1/include/boost")

#添加头文件搜索路径
include_directories(/usr/local/Cellar/boost/1.78.0_1/include)

#添加库文件搜索路径
link_directories(/usr/local/Cellar/boost/1.78.0_1/lib)

#用于将当前目录下的所有源文件的名字保存在变量 DIR_SRCS 中
#aux_source_directory(. DIR_SRCS)
#add_executable(Demo ${DIR_SRCS})
#在这里根据名字boost_thread去寻找libboost_thread.a文件
#target_link_libraries(demo boost_thread boost_system)

add_executable(Demo main.cpp TestA.cpp TestA.h TestMain.cpp Clock.cpp Clock.h StringA.cpp StringA.h CountedObject.cpp CountedObject.h FriendTestMain.cpp Television.cpp Television.h TeleController.cpp TeleController.h Complex.cpp Complex.h complexTestMain.cpp Integer.cpp Integer.h OperatorTestMain.cpp StringTestMain.cpp StringUtil.cpp StringUtil.h vectorTestMain.cpp mapTestMain.cpp CStackTestMain.cpp CPPStackTestMain.cpp InheritTestMain.cpp InheritMainTest.cpp MultiInheritTestMain.cpp VirtualBTableTestMain.cpp VirtualFunTestMain.cpp VirtualFunTableTestMain.cpp AbstractVirtualTestMain.cpp PureDestructFunTestMain.cpp DynamicTestMain.cpp CShape.cpp CShape.h DynamicBase.h ExceptionTestMain.cpp SingletonTestMain.cpp SingletonAutoPtrTestMain.cpp iostreamTestMain.cpp TemplateTestMain.cpp max.cpp max.h ClassTemplateTestMain.cpp Stack.h Stack2.h Stack3.h SingletonTemplateTestMain.cpp SingletonTemplate.h STLTestMain.cpp STLFunObjectTestMain.cpp STLAlogrithmTestMain.cpp STLAdapterTestMain.cpp STLIteratorAdapterTestMain.cpp STLContainerAdapterTestMain.cpp STLBootPointerTestMain.cpp ThreadTestMain.cpp MemoryTestMain.cpp Memory2TestMain.cpp CppNewFeatureTestMain.cpp STLBoostWeakPtrTestMain.cpp STLBoostSharedPtrTestMain.cpp ThreadPTestMain.cpp InterviewTestMain.cpp XMsgServer.cpp XMsgServer.h XMThread.cpp XMThread.h)

