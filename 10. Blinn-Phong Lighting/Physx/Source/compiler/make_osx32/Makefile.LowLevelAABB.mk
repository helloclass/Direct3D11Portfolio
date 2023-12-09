# Makefile generated by XPJ for OSX32
-include Makefile.custom
ProjectName = LowLevelAABB
LowLevelAABB_custom_cflags := -isysroot $(APPLE_OSX_SDK_CURRENT_VERSION)
LowLevelAABB_custom_lflags := -isysroot $(APPLE_OSX_SDK_CURRENT_VERSION)
LowLevelAABB_cppfiles   += ./../../LowLevelAABB/src/BpBroadPhase.cpp
LowLevelAABB_cppfiles   += ./../../LowLevelAABB/src/BpBroadPhaseMBP.cpp
LowLevelAABB_cppfiles   += ./../../LowLevelAABB/src/BpBroadPhaseSap.cpp
LowLevelAABB_cppfiles   += ./../../LowLevelAABB/src/BpBroadPhaseSapAux.cpp
LowLevelAABB_cppfiles   += ./../../LowLevelAABB/src/BpMBPTasks.cpp
LowLevelAABB_cppfiles   += ./../../LowLevelAABB/src/BpSAPTasks.cpp
LowLevelAABB_cppfiles   += ./../../LowLevelAABB/src/BpSimpleAABBManager.cpp

LowLevelAABB_cpp_debug_dep    = $(addprefix $(DEPSDIR)/LowLevelAABB/debug/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(LowLevelAABB_cppfiles)))))
LowLevelAABB_cc_debug_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.debug.P, $(LowLevelAABB_ccfiles)))))
LowLevelAABB_c_debug_dep      = $(addprefix $(DEPSDIR)/LowLevelAABB/debug/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(LowLevelAABB_cfiles)))))
LowLevelAABB_debug_dep      = $(LowLevelAABB_cpp_debug_dep) $(LowLevelAABB_cc_debug_dep) $(LowLevelAABB_c_debug_dep)
-include $(LowLevelAABB_debug_dep)
LowLevelAABB_cpp_checked_dep    = $(addprefix $(DEPSDIR)/LowLevelAABB/checked/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(LowLevelAABB_cppfiles)))))
LowLevelAABB_cc_checked_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.checked.P, $(LowLevelAABB_ccfiles)))))
LowLevelAABB_c_checked_dep      = $(addprefix $(DEPSDIR)/LowLevelAABB/checked/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(LowLevelAABB_cfiles)))))
LowLevelAABB_checked_dep      = $(LowLevelAABB_cpp_checked_dep) $(LowLevelAABB_cc_checked_dep) $(LowLevelAABB_c_checked_dep)
-include $(LowLevelAABB_checked_dep)
LowLevelAABB_cpp_profile_dep    = $(addprefix $(DEPSDIR)/LowLevelAABB/profile/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(LowLevelAABB_cppfiles)))))
LowLevelAABB_cc_profile_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.profile.P, $(LowLevelAABB_ccfiles)))))
LowLevelAABB_c_profile_dep      = $(addprefix $(DEPSDIR)/LowLevelAABB/profile/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(LowLevelAABB_cfiles)))))
LowLevelAABB_profile_dep      = $(LowLevelAABB_cpp_profile_dep) $(LowLevelAABB_cc_profile_dep) $(LowLevelAABB_c_profile_dep)
-include $(LowLevelAABB_profile_dep)
LowLevelAABB_cpp_release_dep    = $(addprefix $(DEPSDIR)/LowLevelAABB/release/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.P, $(LowLevelAABB_cppfiles)))))
LowLevelAABB_cc_release_dep    = $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.release.P, $(LowLevelAABB_ccfiles)))))
LowLevelAABB_c_release_dep      = $(addprefix $(DEPSDIR)/LowLevelAABB/release/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.P, $(LowLevelAABB_cfiles)))))
LowLevelAABB_release_dep      = $(LowLevelAABB_cpp_release_dep) $(LowLevelAABB_cc_release_dep) $(LowLevelAABB_c_release_dep)
-include $(LowLevelAABB_release_dep)
LowLevelAABB_debug_hpaths    := 
LowLevelAABB_debug_hpaths    += ./../../Common/include
LowLevelAABB_debug_hpaths    += ./../../../../PxShared/include
LowLevelAABB_debug_hpaths    += ./../../../../PxShared/src/foundation/include
LowLevelAABB_debug_hpaths    += ./../../../../PxShared/src/fastxml/include
LowLevelAABB_debug_hpaths    += ./../../../../PxShared/src/pvd/include
LowLevelAABB_debug_hpaths    += ./../../../Include
LowLevelAABB_debug_hpaths    += ./../../../Include/common
LowLevelAABB_debug_hpaths    += ./../../../Include/geometry
LowLevelAABB_debug_hpaths    += ./../../../Include/GeomUtils
LowLevelAABB_debug_hpaths    += ./../../Common/src/unix
LowLevelAABB_debug_hpaths    += ./../../GeomUtils/headers
LowLevelAABB_debug_hpaths    += ./../../GeomUtils/src
LowLevelAABB_debug_hpaths    += ./../../Common/src
LowLevelAABB_debug_hpaths    += ./../../LowLevel/API/include
LowLevelAABB_debug_hpaths    += ./../../LowLevel/common/include/utils
LowLevelAABB_debug_hpaths    += ./../../LowLevel/common/include/pipeline
LowLevelAABB_debug_hpaths    += ./../../LowLevelAABB/include
LowLevelAABB_debug_hpaths    += ./../../LowLevelAABB/src
LowLevelAABB_debug_hpaths    += ./../../GpuBroadPhase/include
LowLevelAABB_debug_hpaths    += ./../../GpuBroadPhase/src
LowLevelAABB_debug_hpaths    += ./../../LowLevelAABB/unix/include
LowLevelAABB_debug_lpaths    := 
LowLevelAABB_debug_defines   := $(LowLevelAABB_custom_defines)
LowLevelAABB_debug_defines   += PX_PHYSX_STATIC_LIB
LowLevelAABB_debug_defines   += _DEBUG
LowLevelAABB_debug_defines   += PX_DEBUG=1
LowLevelAABB_debug_defines   += PX_CHECKED=1
LowLevelAABB_debug_defines   += PX_SUPPORT_PVD=1
LowLevelAABB_debug_libraries := 
LowLevelAABB_debug_common_cflags	:= $(LowLevelAABB_custom_cflags)
LowLevelAABB_debug_common_cflags    += -MMD
LowLevelAABB_debug_common_cflags    += $(addprefix -D, $(LowLevelAABB_debug_defines))
LowLevelAABB_debug_common_cflags    += $(addprefix -I, $(LowLevelAABB_debug_hpaths))
LowLevelAABB_debug_cflags	:= $(LowLevelAABB_debug_common_cflags)
LowLevelAABB_debug_cflags  += -arch i386
LowLevelAABB_debug_cflags  += -pipe -mmacosx-version-min=10.7 -msse2 -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -Werror
LowLevelAABB_debug_cflags  += -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -pedantic -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables
LowLevelAABB_debug_cflags  += -Wno-cast-align -Wno-conversion -Wno-missing-variable-declarations -Wno-shift-sign-overflow
LowLevelAABB_debug_cflags  += -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-invalid-offsetof
LowLevelAABB_debug_cflags  += -Wno-c++11-extensions
LowLevelAABB_debug_cflags  += -g3 -gdwarf-2 -O0
LowLevelAABB_debug_cppflags	:= $(LowLevelAABB_debug_common_cflags)
LowLevelAABB_debug_cppflags  += -arch i386
LowLevelAABB_debug_cppflags  += -pipe -mmacosx-version-min=10.7 -msse2 -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -Werror
LowLevelAABB_debug_cppflags  += -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -pedantic -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables
LowLevelAABB_debug_cppflags  += -Wno-cast-align -Wno-conversion -Wno-missing-variable-declarations -Wno-shift-sign-overflow
LowLevelAABB_debug_cppflags  += -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-invalid-offsetof
LowLevelAABB_debug_cppflags  += -Wno-c++11-extensions
LowLevelAABB_debug_cppflags  += -g3 -gdwarf-2 -O0
LowLevelAABB_debug_lflags    := $(LowLevelAABB_custom_lflags)
LowLevelAABB_debug_lflags    += $(addprefix -L, $(LowLevelAABB_debug_lpaths))
LowLevelAABB_debug_lflags  += $(addprefix -l, $(LowLevelAABB_debug_libraries))
LowLevelAABB_debug_lflags  += -arch i386
LowLevelAABB_debug_objsdir  = $(OBJS_DIR)/LowLevelAABB_debug
LowLevelAABB_debug_cpp_o    = $(addprefix $(LowLevelAABB_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(LowLevelAABB_cppfiles)))))
LowLevelAABB_debug_cc_o    = $(addprefix $(LowLevelAABB_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(LowLevelAABB_ccfiles)))))
LowLevelAABB_debug_c_o      = $(addprefix $(LowLevelAABB_debug_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(LowLevelAABB_cfiles)))))
LowLevelAABB_debug_obj      = $(LowLevelAABB_debug_cpp_o) $(LowLevelAABB_debug_cc_o) $(LowLevelAABB_debug_c_o)
LowLevelAABB_debug_bin      := ./../../../Lib/osx32/libLowLevelAABBDEBUG.a

clean_LowLevelAABB_debug: 
	@$(ECHO) clean LowLevelAABB debug
	@$(RMDIR) $(LowLevelAABB_debug_objsdir)
	@$(RMDIR) $(LowLevelAABB_debug_bin)
	@$(RMDIR) $(DEPSDIR)/LowLevelAABB/debug

build_LowLevelAABB_debug: postbuild_LowLevelAABB_debug
postbuild_LowLevelAABB_debug: mainbuild_LowLevelAABB_debug
mainbuild_LowLevelAABB_debug: prebuild_LowLevelAABB_debug $(LowLevelAABB_debug_bin)
prebuild_LowLevelAABB_debug:

$(LowLevelAABB_debug_bin): $(LowLevelAABB_debug_obj) 
	mkdir -p `dirname ./../../../Lib/osx32/libLowLevelAABBDEBUG.a`
	@$(AR) rcs $(LowLevelAABB_debug_bin) $(LowLevelAABB_debug_obj)
	$(ECHO) building $@ complete!

LowLevelAABB_debug_DEPDIR = $(dir $(@))/$(*F)
$(LowLevelAABB_debug_cpp_o): $(LowLevelAABB_debug_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling debug $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(LowLevelAABB_debug_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/LowLevelAABB/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cppfiles))))))
	cp $(LowLevelAABB_debug_DEPDIR).d $(addprefix $(DEPSDIR)/LowLevelAABB/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/LowLevelAABB/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cppfiles))))).P; \
	  rm -f $(LowLevelAABB_debug_DEPDIR).d

$(LowLevelAABB_debug_cc_o): $(LowLevelAABB_debug_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling debug $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(LowLevelAABB_debug_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_ccfiles))))))
	cp $(LowLevelAABB_debug_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_ccfiles))))).debug.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_ccfiles))))).debug.P; \
	  rm -f $(LowLevelAABB_debug_DEPDIR).d

$(LowLevelAABB_debug_c_o): $(LowLevelAABB_debug_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling debug $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(LowLevelAABB_debug_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/LowLevelAABB/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cfiles))))))
	cp $(LowLevelAABB_debug_DEPDIR).d $(addprefix $(DEPSDIR)/LowLevelAABB/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_debug_DEPDIR).d >> $(addprefix $(DEPSDIR)/LowLevelAABB/debug/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_debug_objsdir),, $@))), $(LowLevelAABB_cfiles))))).P; \
	  rm -f $(LowLevelAABB_debug_DEPDIR).d

LowLevelAABB_checked_hpaths    := 
LowLevelAABB_checked_hpaths    += ./../../Common/include
LowLevelAABB_checked_hpaths    += ./../../../../PxShared/include
LowLevelAABB_checked_hpaths    += ./../../../../PxShared/src/foundation/include
LowLevelAABB_checked_hpaths    += ./../../../../PxShared/src/fastxml/include
LowLevelAABB_checked_hpaths    += ./../../../../PxShared/src/pvd/include
LowLevelAABB_checked_hpaths    += ./../../../Include
LowLevelAABB_checked_hpaths    += ./../../../Include/common
LowLevelAABB_checked_hpaths    += ./../../../Include/geometry
LowLevelAABB_checked_hpaths    += ./../../../Include/GeomUtils
LowLevelAABB_checked_hpaths    += ./../../Common/src/unix
LowLevelAABB_checked_hpaths    += ./../../GeomUtils/headers
LowLevelAABB_checked_hpaths    += ./../../GeomUtils/src
LowLevelAABB_checked_hpaths    += ./../../Common/src
LowLevelAABB_checked_hpaths    += ./../../LowLevel/API/include
LowLevelAABB_checked_hpaths    += ./../../LowLevel/common/include/utils
LowLevelAABB_checked_hpaths    += ./../../LowLevel/common/include/pipeline
LowLevelAABB_checked_hpaths    += ./../../LowLevelAABB/include
LowLevelAABB_checked_hpaths    += ./../../LowLevelAABB/src
LowLevelAABB_checked_hpaths    += ./../../GpuBroadPhase/include
LowLevelAABB_checked_hpaths    += ./../../GpuBroadPhase/src
LowLevelAABB_checked_hpaths    += ./../../LowLevelAABB/unix/include
LowLevelAABB_checked_lpaths    := 
LowLevelAABB_checked_defines   := $(LowLevelAABB_custom_defines)
LowLevelAABB_checked_defines   += PX_PHYSX_STATIC_LIB
LowLevelAABB_checked_defines   += NDEBUG
LowLevelAABB_checked_defines   += PX_CHECKED=1
LowLevelAABB_checked_defines   += PX_SUPPORT_PVD=1
LowLevelAABB_checked_libraries := 
LowLevelAABB_checked_common_cflags	:= $(LowLevelAABB_custom_cflags)
LowLevelAABB_checked_common_cflags    += -MMD
LowLevelAABB_checked_common_cflags    += $(addprefix -D, $(LowLevelAABB_checked_defines))
LowLevelAABB_checked_common_cflags    += $(addprefix -I, $(LowLevelAABB_checked_hpaths))
LowLevelAABB_checked_cflags	:= $(LowLevelAABB_checked_common_cflags)
LowLevelAABB_checked_cflags  += -arch i386
LowLevelAABB_checked_cflags  += -pipe -mmacosx-version-min=10.7 -msse2 -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -Werror
LowLevelAABB_checked_cflags  += -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -pedantic -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables
LowLevelAABB_checked_cflags  += -Wno-cast-align -Wno-conversion -Wno-missing-variable-declarations -Wno-shift-sign-overflow
LowLevelAABB_checked_cflags  += -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-invalid-offsetof
LowLevelAABB_checked_cflags  += -Wno-c++11-extensions
LowLevelAABB_checked_cflags  += -g3 -gdwarf-2 -O3 -fno-strict-aliasing
LowLevelAABB_checked_cppflags	:= $(LowLevelAABB_checked_common_cflags)
LowLevelAABB_checked_cppflags  += -arch i386
LowLevelAABB_checked_cppflags  += -pipe -mmacosx-version-min=10.7 -msse2 -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -Werror
LowLevelAABB_checked_cppflags  += -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -pedantic -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables
LowLevelAABB_checked_cppflags  += -Wno-cast-align -Wno-conversion -Wno-missing-variable-declarations -Wno-shift-sign-overflow
LowLevelAABB_checked_cppflags  += -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-invalid-offsetof
LowLevelAABB_checked_cppflags  += -Wno-c++11-extensions
LowLevelAABB_checked_cppflags  += -g3 -gdwarf-2 -O3 -fno-strict-aliasing
LowLevelAABB_checked_lflags    := $(LowLevelAABB_custom_lflags)
LowLevelAABB_checked_lflags    += $(addprefix -L, $(LowLevelAABB_checked_lpaths))
LowLevelAABB_checked_lflags  += $(addprefix -l, $(LowLevelAABB_checked_libraries))
LowLevelAABB_checked_lflags  += -arch i386
LowLevelAABB_checked_objsdir  = $(OBJS_DIR)/LowLevelAABB_checked
LowLevelAABB_checked_cpp_o    = $(addprefix $(LowLevelAABB_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(LowLevelAABB_cppfiles)))))
LowLevelAABB_checked_cc_o    = $(addprefix $(LowLevelAABB_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(LowLevelAABB_ccfiles)))))
LowLevelAABB_checked_c_o      = $(addprefix $(LowLevelAABB_checked_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(LowLevelAABB_cfiles)))))
LowLevelAABB_checked_obj      = $(LowLevelAABB_checked_cpp_o) $(LowLevelAABB_checked_cc_o) $(LowLevelAABB_checked_c_o)
LowLevelAABB_checked_bin      := ./../../../Lib/osx32/libLowLevelAABBCHECKED.a

clean_LowLevelAABB_checked: 
	@$(ECHO) clean LowLevelAABB checked
	@$(RMDIR) $(LowLevelAABB_checked_objsdir)
	@$(RMDIR) $(LowLevelAABB_checked_bin)
	@$(RMDIR) $(DEPSDIR)/LowLevelAABB/checked

build_LowLevelAABB_checked: postbuild_LowLevelAABB_checked
postbuild_LowLevelAABB_checked: mainbuild_LowLevelAABB_checked
mainbuild_LowLevelAABB_checked: prebuild_LowLevelAABB_checked $(LowLevelAABB_checked_bin)
prebuild_LowLevelAABB_checked:

$(LowLevelAABB_checked_bin): $(LowLevelAABB_checked_obj) 
	mkdir -p `dirname ./../../../Lib/osx32/libLowLevelAABBCHECKED.a`
	@$(AR) rcs $(LowLevelAABB_checked_bin) $(LowLevelAABB_checked_obj)
	$(ECHO) building $@ complete!

LowLevelAABB_checked_DEPDIR = $(dir $(@))/$(*F)
$(LowLevelAABB_checked_cpp_o): $(LowLevelAABB_checked_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling checked $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(LowLevelAABB_checked_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/LowLevelAABB/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cppfiles))))))
	cp $(LowLevelAABB_checked_DEPDIR).d $(addprefix $(DEPSDIR)/LowLevelAABB/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/LowLevelAABB/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cppfiles))))).P; \
	  rm -f $(LowLevelAABB_checked_DEPDIR).d

$(LowLevelAABB_checked_cc_o): $(LowLevelAABB_checked_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling checked $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(LowLevelAABB_checked_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_ccfiles))))))
	cp $(LowLevelAABB_checked_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_ccfiles))))).checked.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_ccfiles))))).checked.P; \
	  rm -f $(LowLevelAABB_checked_DEPDIR).d

$(LowLevelAABB_checked_c_o): $(LowLevelAABB_checked_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling checked $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(LowLevelAABB_checked_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/LowLevelAABB/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cfiles))))))
	cp $(LowLevelAABB_checked_DEPDIR).d $(addprefix $(DEPSDIR)/LowLevelAABB/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_checked_DEPDIR).d >> $(addprefix $(DEPSDIR)/LowLevelAABB/checked/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_checked_objsdir),, $@))), $(LowLevelAABB_cfiles))))).P; \
	  rm -f $(LowLevelAABB_checked_DEPDIR).d

LowLevelAABB_profile_hpaths    := 
LowLevelAABB_profile_hpaths    += ./../../Common/include
LowLevelAABB_profile_hpaths    += ./../../../../PxShared/include
LowLevelAABB_profile_hpaths    += ./../../../../PxShared/src/foundation/include
LowLevelAABB_profile_hpaths    += ./../../../../PxShared/src/fastxml/include
LowLevelAABB_profile_hpaths    += ./../../../../PxShared/src/pvd/include
LowLevelAABB_profile_hpaths    += ./../../../Include
LowLevelAABB_profile_hpaths    += ./../../../Include/common
LowLevelAABB_profile_hpaths    += ./../../../Include/geometry
LowLevelAABB_profile_hpaths    += ./../../../Include/GeomUtils
LowLevelAABB_profile_hpaths    += ./../../Common/src/unix
LowLevelAABB_profile_hpaths    += ./../../GeomUtils/headers
LowLevelAABB_profile_hpaths    += ./../../GeomUtils/src
LowLevelAABB_profile_hpaths    += ./../../Common/src
LowLevelAABB_profile_hpaths    += ./../../LowLevel/API/include
LowLevelAABB_profile_hpaths    += ./../../LowLevel/common/include/utils
LowLevelAABB_profile_hpaths    += ./../../LowLevel/common/include/pipeline
LowLevelAABB_profile_hpaths    += ./../../LowLevelAABB/include
LowLevelAABB_profile_hpaths    += ./../../LowLevelAABB/src
LowLevelAABB_profile_hpaths    += ./../../GpuBroadPhase/include
LowLevelAABB_profile_hpaths    += ./../../GpuBroadPhase/src
LowLevelAABB_profile_hpaths    += ./../../LowLevelAABB/unix/include
LowLevelAABB_profile_lpaths    := 
LowLevelAABB_profile_defines   := $(LowLevelAABB_custom_defines)
LowLevelAABB_profile_defines   += PX_PHYSX_STATIC_LIB
LowLevelAABB_profile_defines   += NDEBUG
LowLevelAABB_profile_defines   += PX_PROFILE=1
LowLevelAABB_profile_defines   += PX_SUPPORT_PVD=1
LowLevelAABB_profile_libraries := 
LowLevelAABB_profile_common_cflags	:= $(LowLevelAABB_custom_cflags)
LowLevelAABB_profile_common_cflags    += -MMD
LowLevelAABB_profile_common_cflags    += $(addprefix -D, $(LowLevelAABB_profile_defines))
LowLevelAABB_profile_common_cflags    += $(addprefix -I, $(LowLevelAABB_profile_hpaths))
LowLevelAABB_profile_cflags	:= $(LowLevelAABB_profile_common_cflags)
LowLevelAABB_profile_cflags  += -arch i386
LowLevelAABB_profile_cflags  += -pipe -mmacosx-version-min=10.7 -msse2 -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -Werror
LowLevelAABB_profile_cflags  += -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -pedantic -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables
LowLevelAABB_profile_cflags  += -Wno-cast-align -Wno-conversion -Wno-missing-variable-declarations -Wno-shift-sign-overflow
LowLevelAABB_profile_cflags  += -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-invalid-offsetof
LowLevelAABB_profile_cflags  += -Wno-c++11-extensions
LowLevelAABB_profile_cflags  += -O3 -fno-strict-aliasing
LowLevelAABB_profile_cppflags	:= $(LowLevelAABB_profile_common_cflags)
LowLevelAABB_profile_cppflags  += -arch i386
LowLevelAABB_profile_cppflags  += -pipe -mmacosx-version-min=10.7 -msse2 -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -Werror
LowLevelAABB_profile_cppflags  += -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -pedantic -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables
LowLevelAABB_profile_cppflags  += -Wno-cast-align -Wno-conversion -Wno-missing-variable-declarations -Wno-shift-sign-overflow
LowLevelAABB_profile_cppflags  += -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-invalid-offsetof
LowLevelAABB_profile_cppflags  += -Wno-c++11-extensions
LowLevelAABB_profile_cppflags  += -O3 -fno-strict-aliasing
LowLevelAABB_profile_lflags    := $(LowLevelAABB_custom_lflags)
LowLevelAABB_profile_lflags    += $(addprefix -L, $(LowLevelAABB_profile_lpaths))
LowLevelAABB_profile_lflags  += $(addprefix -l, $(LowLevelAABB_profile_libraries))
LowLevelAABB_profile_lflags  += -arch i386
LowLevelAABB_profile_objsdir  = $(OBJS_DIR)/LowLevelAABB_profile
LowLevelAABB_profile_cpp_o    = $(addprefix $(LowLevelAABB_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(LowLevelAABB_cppfiles)))))
LowLevelAABB_profile_cc_o    = $(addprefix $(LowLevelAABB_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(LowLevelAABB_ccfiles)))))
LowLevelAABB_profile_c_o      = $(addprefix $(LowLevelAABB_profile_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(LowLevelAABB_cfiles)))))
LowLevelAABB_profile_obj      = $(LowLevelAABB_profile_cpp_o) $(LowLevelAABB_profile_cc_o) $(LowLevelAABB_profile_c_o)
LowLevelAABB_profile_bin      := ./../../../Lib/osx32/libLowLevelAABBPROFILE.a

clean_LowLevelAABB_profile: 
	@$(ECHO) clean LowLevelAABB profile
	@$(RMDIR) $(LowLevelAABB_profile_objsdir)
	@$(RMDIR) $(LowLevelAABB_profile_bin)
	@$(RMDIR) $(DEPSDIR)/LowLevelAABB/profile

build_LowLevelAABB_profile: postbuild_LowLevelAABB_profile
postbuild_LowLevelAABB_profile: mainbuild_LowLevelAABB_profile
mainbuild_LowLevelAABB_profile: prebuild_LowLevelAABB_profile $(LowLevelAABB_profile_bin)
prebuild_LowLevelAABB_profile:

$(LowLevelAABB_profile_bin): $(LowLevelAABB_profile_obj) 
	mkdir -p `dirname ./../../../Lib/osx32/libLowLevelAABBPROFILE.a`
	@$(AR) rcs $(LowLevelAABB_profile_bin) $(LowLevelAABB_profile_obj)
	$(ECHO) building $@ complete!

LowLevelAABB_profile_DEPDIR = $(dir $(@))/$(*F)
$(LowLevelAABB_profile_cpp_o): $(LowLevelAABB_profile_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling profile $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(LowLevelAABB_profile_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/LowLevelAABB/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cppfiles))))))
	cp $(LowLevelAABB_profile_DEPDIR).d $(addprefix $(DEPSDIR)/LowLevelAABB/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/LowLevelAABB/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cppfiles))))).P; \
	  rm -f $(LowLevelAABB_profile_DEPDIR).d

$(LowLevelAABB_profile_cc_o): $(LowLevelAABB_profile_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling profile $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(LowLevelAABB_profile_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_ccfiles))))))
	cp $(LowLevelAABB_profile_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_ccfiles))))).profile.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_ccfiles))))).profile.P; \
	  rm -f $(LowLevelAABB_profile_DEPDIR).d

$(LowLevelAABB_profile_c_o): $(LowLevelAABB_profile_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling profile $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(LowLevelAABB_profile_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/LowLevelAABB/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cfiles))))))
	cp $(LowLevelAABB_profile_DEPDIR).d $(addprefix $(DEPSDIR)/LowLevelAABB/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_profile_DEPDIR).d >> $(addprefix $(DEPSDIR)/LowLevelAABB/profile/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_profile_objsdir),, $@))), $(LowLevelAABB_cfiles))))).P; \
	  rm -f $(LowLevelAABB_profile_DEPDIR).d

LowLevelAABB_release_hpaths    := 
LowLevelAABB_release_hpaths    += ./../../Common/include
LowLevelAABB_release_hpaths    += ./../../../../PxShared/include
LowLevelAABB_release_hpaths    += ./../../../../PxShared/src/foundation/include
LowLevelAABB_release_hpaths    += ./../../../../PxShared/src/fastxml/include
LowLevelAABB_release_hpaths    += ./../../../../PxShared/src/pvd/include
LowLevelAABB_release_hpaths    += ./../../../Include
LowLevelAABB_release_hpaths    += ./../../../Include/common
LowLevelAABB_release_hpaths    += ./../../../Include/geometry
LowLevelAABB_release_hpaths    += ./../../../Include/GeomUtils
LowLevelAABB_release_hpaths    += ./../../Common/src/unix
LowLevelAABB_release_hpaths    += ./../../GeomUtils/headers
LowLevelAABB_release_hpaths    += ./../../GeomUtils/src
LowLevelAABB_release_hpaths    += ./../../Common/src
LowLevelAABB_release_hpaths    += ./../../LowLevel/API/include
LowLevelAABB_release_hpaths    += ./../../LowLevel/common/include/utils
LowLevelAABB_release_hpaths    += ./../../LowLevel/common/include/pipeline
LowLevelAABB_release_hpaths    += ./../../LowLevelAABB/include
LowLevelAABB_release_hpaths    += ./../../LowLevelAABB/src
LowLevelAABB_release_hpaths    += ./../../GpuBroadPhase/include
LowLevelAABB_release_hpaths    += ./../../GpuBroadPhase/src
LowLevelAABB_release_hpaths    += ./../../LowLevelAABB/unix/include
LowLevelAABB_release_lpaths    := 
LowLevelAABB_release_defines   := $(LowLevelAABB_custom_defines)
LowLevelAABB_release_defines   += PX_PHYSX_STATIC_LIB
LowLevelAABB_release_defines   += NDEBUG
LowLevelAABB_release_defines   += PX_SUPPORT_PVD=0
LowLevelAABB_release_libraries := 
LowLevelAABB_release_common_cflags	:= $(LowLevelAABB_custom_cflags)
LowLevelAABB_release_common_cflags    += -MMD
LowLevelAABB_release_common_cflags    += $(addprefix -D, $(LowLevelAABB_release_defines))
LowLevelAABB_release_common_cflags    += $(addprefix -I, $(LowLevelAABB_release_hpaths))
LowLevelAABB_release_cflags	:= $(LowLevelAABB_release_common_cflags)
LowLevelAABB_release_cflags  += -arch i386
LowLevelAABB_release_cflags  += -pipe -mmacosx-version-min=10.7 -msse2 -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -Werror
LowLevelAABB_release_cflags  += -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -pedantic -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables
LowLevelAABB_release_cflags  += -Wno-cast-align -Wno-conversion -Wno-missing-variable-declarations -Wno-shift-sign-overflow
LowLevelAABB_release_cflags  += -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-invalid-offsetof
LowLevelAABB_release_cflags  += -Wno-c++11-extensions
LowLevelAABB_release_cflags  += -O3 -fno-strict-aliasing
LowLevelAABB_release_cppflags	:= $(LowLevelAABB_release_common_cflags)
LowLevelAABB_release_cppflags  += -arch i386
LowLevelAABB_release_cppflags  += -pipe -mmacosx-version-min=10.7 -msse2 -fno-exceptions -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -Werror
LowLevelAABB_release_cppflags  += -Wall -Wextra -fstrict-aliasing -Wstrict-aliasing=2 -pedantic -Weverything -Wno-documentation-deprecated-sync -Wno-documentation-unknown-command -Wno-float-equal -Wno-padded -Wno-weak-vtables
LowLevelAABB_release_cppflags  += -Wno-cast-align -Wno-conversion -Wno-missing-variable-declarations -Wno-shift-sign-overflow
LowLevelAABB_release_cppflags  += -Wno-exit-time-destructors -Wno-global-constructors -Wno-missing-prototypes -Wno-unreachable-code -Wno-unused-macros -Wno-used-but-marked-unused -Wno-weak-template-vtables -Wno-invalid-offsetof
LowLevelAABB_release_cppflags  += -Wno-c++11-extensions
LowLevelAABB_release_cppflags  += -O3 -fno-strict-aliasing
LowLevelAABB_release_lflags    := $(LowLevelAABB_custom_lflags)
LowLevelAABB_release_lflags    += $(addprefix -L, $(LowLevelAABB_release_lpaths))
LowLevelAABB_release_lflags  += $(addprefix -l, $(LowLevelAABB_release_libraries))
LowLevelAABB_release_lflags  += -arch i386
LowLevelAABB_release_objsdir  = $(OBJS_DIR)/LowLevelAABB_release
LowLevelAABB_release_cpp_o    = $(addprefix $(LowLevelAABB_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cpp, %.cpp.o, $(LowLevelAABB_cppfiles)))))
LowLevelAABB_release_cc_o    = $(addprefix $(LowLevelAABB_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.cc, %.cc.o, $(LowLevelAABB_ccfiles)))))
LowLevelAABB_release_c_o      = $(addprefix $(LowLevelAABB_release_objsdir)/, $(subst ./, , $(subst ../, , $(patsubst %.c, %.c.o, $(LowLevelAABB_cfiles)))))
LowLevelAABB_release_obj      = $(LowLevelAABB_release_cpp_o) $(LowLevelAABB_release_cc_o) $(LowLevelAABB_release_c_o)
LowLevelAABB_release_bin      := ./../../../Lib/osx32/libLowLevelAABB.a

clean_LowLevelAABB_release: 
	@$(ECHO) clean LowLevelAABB release
	@$(RMDIR) $(LowLevelAABB_release_objsdir)
	@$(RMDIR) $(LowLevelAABB_release_bin)
	@$(RMDIR) $(DEPSDIR)/LowLevelAABB/release

build_LowLevelAABB_release: postbuild_LowLevelAABB_release
postbuild_LowLevelAABB_release: mainbuild_LowLevelAABB_release
mainbuild_LowLevelAABB_release: prebuild_LowLevelAABB_release $(LowLevelAABB_release_bin)
prebuild_LowLevelAABB_release:

$(LowLevelAABB_release_bin): $(LowLevelAABB_release_obj) 
	mkdir -p `dirname ./../../../Lib/osx32/libLowLevelAABB.a`
	@$(AR) rcs $(LowLevelAABB_release_bin) $(LowLevelAABB_release_obj)
	$(ECHO) building $@ complete!

LowLevelAABB_release_DEPDIR = $(dir $(@))/$(*F)
$(LowLevelAABB_release_cpp_o): $(LowLevelAABB_release_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling release $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cppfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(LowLevelAABB_release_cppflags) -c $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cppfiles)) -o $@
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/LowLevelAABB/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cppfiles))))))
	cp $(LowLevelAABB_release_DEPDIR).d $(addprefix $(DEPSDIR)/LowLevelAABB/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cppfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/LowLevelAABB/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cpp.o,.cpp, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cppfiles))))).P; \
	  rm -f $(LowLevelAABB_release_DEPDIR).d

$(LowLevelAABB_release_cc_o): $(LowLevelAABB_release_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling release $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_ccfiles))...
	mkdir -p $(dir $(@))
	$(CXX) $(LowLevelAABB_release_cppflags) -c $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_ccfiles)) -o $@
	mkdir -p $(dir $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_ccfiles))))))
	cp $(LowLevelAABB_release_DEPDIR).d $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_ccfiles))))).release.P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .cc.o,.cc, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_ccfiles))))).release.P; \
	  rm -f $(LowLevelAABB_release_DEPDIR).d

$(LowLevelAABB_release_c_o): $(LowLevelAABB_release_objsdir)/%.o:
	$(ECHO) LowLevelAABB: compiling release $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cfiles))...
	mkdir -p $(dir $(@))
	$(CC) $(LowLevelAABB_release_cflags) -c $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cfiles)) -o $@ 
	@mkdir -p $(dir $(addprefix $(DEPSDIR)/LowLevelAABB/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cfiles))))))
	cp $(LowLevelAABB_release_DEPDIR).d $(addprefix $(DEPSDIR)/LowLevelAABB/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cfiles))))).P; \
	  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
		-e '/^$$/ d' -e 's/$$/ :/' < $(LowLevelAABB_release_DEPDIR).d >> $(addprefix $(DEPSDIR)/LowLevelAABB/release/, $(subst ./, , $(subst ../, , $(filter %$(strip $(subst .c.o,.c, $(subst $(LowLevelAABB_release_objsdir),, $@))), $(LowLevelAABB_cfiles))))).P; \
	  rm -f $(LowLevelAABB_release_DEPDIR).d

clean_LowLevelAABB:  clean_LowLevelAABB_debug clean_LowLevelAABB_checked clean_LowLevelAABB_profile clean_LowLevelAABB_release
	rm -rf $(DEPSDIR)

export VERBOSE
ifndef VERBOSE
.SILENT:
endif
