ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

USEFILE=

# Extra include path for libfreetype and for target overrides and patches
EXTRA_INCVPATH+=$(QNX_TARGET)/usr/include/freetype2 \
	$(QNX_TARGET)/../target-override/usr/include \
	/Users/stevenbeeckman/Documents/blackberry/OpenCV/modules/core/include \
	/Users/stevenbeeckman/Documents/blackberry/OpenCV/modules/highgui/include \
	$(QNX_TARGET)/usr/include/c++/4.6.3 \
	$(QNX_TARGET)/usr/include/c++/4.6.3/arm-unknown-nto-qnx8.0.0eabi

# Extra library search path for target overrides and patches
EXTRA_LIBVPATH+=$(QNX_TARGET)/../target-override/$(CPUVARDIR)/lib \
	$(QNX_TARGET)/../target-override/$(CPUVARDIR)/usr/lib \
	/Users/stevenbeeckman/Documents/blackberry/OpenCV-QNX/lib

# Compiler options for enhanced security and recording the compiler options in release builds
CCFLAGS+=-fstack-protector-strong -D_FORTIFY_SOURCE=2 \
	$(if $(filter g so shared,$(VARIANTS)),,-fPIE) \
	$(if $(filter g,$(VARIANTS)),,-frecord-gcc-switches)

# Linker options for enhanced security
LDFLAGS+=-Wl,-z,relro -Wl,-z,now $(if $(filter g so shared,$(VARIANTS)),,-pie)

# Add your required library names, here
LIBS+=bps screen m opencv_core

include $(MKFILES_ROOT)/qmacros.mk

# Suppress the _g suffix from the debug variant
BUILDNAME=$(IMAGE_PREF_$(BUILD_TYPE))$(NAME)$(IMAGE_SUFF_$(BUILD_TYPE))

include $(MKFILES_ROOT)/qtargets.mk

OPTIMIZE_TYPE_g=none
OPTIMIZE_TYPE=$(OPTIMIZE_TYPE_$(filter g, $(VARIANTS)))
