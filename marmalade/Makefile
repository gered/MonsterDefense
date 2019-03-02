#-------------------------------------------------------------------------------
# automatically-figured-out build settings
#-------------------------------------------------------------------------------

# the below 2 variables are figured out under the assumption that this whole
# project has it's directories set up and located like the following:
#
#    Project root: /path/to/PROJECT_NAME
#    Marmalade MKB: /path/to/PROJECT_NAME/marmalade/PROJECT_NAME.mkb
#
# Where PROJECT_NAME is figured out automatically below under these assumptions

PROJECT_NAME := $(shell pushd . > /dev/null; cd ..; basename $$(pwd); popd > /dev/null)
LC_PROJECT_NAME := $(shell echo $(PROJECT_NAME) | tr A-Z a-z)

# We convert PROJECT_NAME to lowercase as some Marmalade deployment stuff 
# assumes that the build dirs have the MKB filename as lowercase in the 
# directory name (the deploy_config.py files that are automatically generated
# during builds always have paths in them that look like that, regardless of
# what the --builddir value might have been)

MKB := $(PROJECT_NAME).mkb



#-------------------------------------------------------------------------------
# build configuration settings
#-------------------------------------------------------------------------------

# check "config" argument (default is "debug")
ifndef ("config")
config=debug
endif

ifeq ("$(config)","debug")
BUILD_CONFIG := --debug
else ifeq ("$(config)","release")
BUILD_CONFIG := --release
else 
$(error Invalid configuration specified via "config" argument. Should be "debug" or "release")
endif

# check "os" argument (default is "simulator")
ifndef ("os")
os=simulator
endif

ifeq ("$(os)","simulator")
# nothing
OS := 
else ifeq ("$(os)","android")
OS := --os=android
else ifeq ("$(os)","ios")
OS := --os=iphone
else
$(error Invalid OS specified via "os" argument. Should be "simulator", "android" or "ios")
endif

# check "arch" argument (default is "x86", unless building for android/ios, where we set "arm")
ifndef ("arch")
ifeq ("$(os)","android")
arch=arm
else ifeq ("$(os)","ios")
arch=arm
else
arch=x86
endif
endif

ifeq ("$(arch)","x86")
ARCH := --x86
BUILD_DIR := build_$(LC_PROJECT_NAME)_scons_x86
else ifeq ("$(arch)","arm")
ARCH := --arm
BUILD_DIR := build_$(LC_PROJECT_NAME)_scons_arm
else
$(error Invalid architecture specified via "arch" argument. Should be "x86" or "arm")
endif


S3E_BIN := $(MARMALADE_ROOT)/s3e/bin



$(info ===== PROJECT=$(PROJECT_NAME) CONFIG=$(config) ARCH=$(arch) OS=$(os) =====)


all: check-env binary

#-------------------------------------------------------------------------------
mkb: check-env
	$(info ===== MKB GENERATE =====)
	@./mkb_generate

#-------------------------------------------------------------------------------
binary: check-env mkb
	$(info ===== BUILD =====)
	@$(S3E_BIN)/mkb $(MKB) $(BUILD_CONFIG) $(ARCH) --buildenv=scons --builddir=$(BUILD_DIR)
	
#-------------------------------------------------------------------------------
clean: check-env
	$(info ===== CLEAN =====)
	@$(S3E_BIN)/mkb $(MKB) $(BUILD_CONFIG) $(ARCH) --buildenv=scons --clean --builddir=$(BUILD_DIR)

#-------------------------------------------------------------------------------
deploy: check-env binary
	$(info ===== DEPLOY TO DEVICE =====)
ifneq ("$(os)","simulator")
	@$(S3E_BIN)/s3e_deploy $(OS) $(BUILD_CONFIG) --install $(BUILD_DIR)/deploy_config.py
endif

#-------------------------------------------------------------------------------
run: check-env deploy
	$(info ===== RUN ON DEVICE =====)
ifeq ("$(os)","simulator")
ifeq ("$(arch)","x86")
	# running in the x86 simulator
	@./run_$(PROJECT_NAME).py
else
	# running in the arm (qemu) simulator
	@./run_$(PROJECT_NAME)_qemu.py
endif
else
	@$(S3E_BIN)/s3e_deploy $(OS) $(BUILD_CONFIG) --run $(BUILD_DIR)/deploy_config.py
endif

check-env:
ifndef MARMALADE_ROOT
	$(error MARMALADE_ROOT must be set to the root of the Marmalade SDK. (eg. "/Developer/Marmalade/6.1"))
endif
