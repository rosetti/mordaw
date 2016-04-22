# Automatically generated makefile, created by the Projucer
# Don't edit this file! Your changes will be overwritten when you re-save the Projucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_6D53C8B4=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ../../JuceLibraryCode -I ../../../../../../../Applications/JUCE/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := KentDAW
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_6D53C8B4=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ../../JuceLibraryCode -I ../../../../../../../Applications/JUCE/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -O3
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := KentDAW
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/SplashImage_48f94350.o \
  $(OBJDIR)/TransportImages_4b3c351d.o \
  $(OBJDIR)/ProjectManager_74d90745.o \
  $(OBJDIR)/Meter_9ce0b876.o \
  $(OBJDIR)/ChannelStripComponent_a4a2f215.o \
  $(OBJDIR)/MixerView_2ce514bb.o \
  $(OBJDIR)/ProjectTab_b868ab9.o \
  $(OBJDIR)/RegionComponent_b9723f56.o \
  $(OBJDIR)/TrackComponent_d82131ef.o \
  $(OBJDIR)/TimelineComponent_3d790929.o \
  $(OBJDIR)/TimelineCursor_f1957674.o \
  $(OBJDIR)/Arrangement_15b9fd77.o \
  $(OBJDIR)/TransportControls_13276ec.o \
  $(OBJDIR)/LeftSide_f50f27fb.o \
  $(OBJDIR)/MainComponent_7cc716b1.o \
  $(OBJDIR)/TopMenu_34ea79c1.o \
  $(OBJDIR)/MainWindow_ca618186.o \
  $(OBJDIR)/Region_abd260a6.o \
  $(OBJDIR)/SampleRegion_ced6b430.o \
  $(OBJDIR)/EQNode_8c8006ae.o \
  $(OBJDIR)/ChannelStripNode_a7e9bf37.o \
  $(OBJDIR)/RecordNode_9064b3f3.o \
  $(OBJDIR)/TrackNode_dfbb5ff7.o \
  $(OBJDIR)/Mixer_d22926d1.o \
  $(OBJDIR)/Track_c2771b13.o \
  $(OBJDIR)/Engine_9fc6a9c4.o \
  $(OBJDIR)/Main_90ebc5c2.o \
  $(OBJDIR)/juce_audio_basics_6b797ca1.o \
  $(OBJDIR)/juce_audio_devices_a742c38b.o \
  $(OBJDIR)/juce_audio_formats_5a29c68a.o \
  $(OBJDIR)/juce_audio_plugin_client_utils_35fbf7.o \
  $(OBJDIR)/juce_audio_processors_dea3173d.o \
  $(OBJDIR)/juce_audio_utils_c7eb679f.o \
  $(OBJDIR)/juce_box2d_95719d14.o \
  $(OBJDIR)/juce_core_75b14332.o \
  $(OBJDIR)/juce_data_structures_72d3da2c.o \
  $(OBJDIR)/juce_events_d2be882c.o \
  $(OBJDIR)/juce_graphics_9c18891e.o \
  $(OBJDIR)/juce_gui_basics_8a6da59c.o \
  $(OBJDIR)/juce_gui_extra_4a026f23.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking KentDAW
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning KentDAW
	@$(CLEANCMD)

strip:
	@echo Stripping KentDAW
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/SplashImage_48f94350.o: ../../Source/Resources/Images/SplashImage.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SplashImage.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TransportImages_4b3c351d.o: ../../Source/Resources/Images/TransportImages.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TransportImages.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ProjectManager_74d90745.o: ../../Source/Core/ProjectManager.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ProjectManager.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Meter_9ce0b876.o: ../../Source/GUI/Meter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Meter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ChannelStripComponent_a4a2f215.o: ../../Source/GUI/ChannelStripComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ChannelStripComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MixerView_2ce514bb.o: ../../Source/GUI/MixerView.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MixerView.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ProjectTab_b868ab9.o: ../../Source/GUI/ProjectTab.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ProjectTab.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RegionComponent_b9723f56.o: ../../Source/GUI/RegionComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RegionComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrackComponent_d82131ef.o: ../../Source/GUI/TrackComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TrackComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TimelineComponent_3d790929.o: ../../Source/GUI/TimelineComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TimelineComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TimelineCursor_f1957674.o: ../../Source/GUI/TimelineCursor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TimelineCursor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Arrangement_15b9fd77.o: ../../Source/GUI/Arrangement.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Arrangement.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TransportControls_13276ec.o: ../../Source/GUI/TransportControls.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TransportControls.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LeftSide_f50f27fb.o: ../../Source/GUI/LeftSide.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LeftSide.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainComponent_7cc716b1.o: ../../Source/GUI/MainComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TopMenu_34ea79c1.o: ../../Source/GUI/TopMenu.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TopMenu.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/MainWindow_ca618186.o: ../../Source/GUI/MainWindow.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling MainWindow.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Region_abd260a6.o: ../../Source/Audio/Regions/Region.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Region.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SampleRegion_ced6b430.o: ../../Source/Audio/Regions/SampleRegion.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SampleRegion.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/EQNode_8c8006ae.o: ../../Source/Audio/Nodes/EQNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling EQNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ChannelStripNode_a7e9bf37.o: ../../Source/Audio/Nodes/ChannelStripNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ChannelStripNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/RecordNode_9064b3f3.o: ../../Source/Audio/Nodes/RecordNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling RecordNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/TrackNode_dfbb5ff7.o: ../../Source/Audio/Nodes/TrackNode.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling TrackNode.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Mixer_d22926d1.o: ../../Source/Audio/Mixer.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Mixer.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Track_c2771b13.o: ../../Source/Audio/Track.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Track.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Engine_9fc6a9c4.o: ../../Source/Audio/Engine.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Engine.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling Main.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_6b797ca1.o: ../../JuceLibraryCode/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_a742c38b.o: ../../JuceLibraryCode/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_5a29c68a.o: ../../JuceLibraryCode/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_plugin_client_utils_35fbf7.o: ../../JuceLibraryCode/juce_audio_plugin_client_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_plugin_client_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_dea3173d.o: ../../JuceLibraryCode/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_utils_c7eb679f.o: ../../JuceLibraryCode/juce_audio_utils.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_utils.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_box2d_95719d14.o: ../../JuceLibraryCode/juce_box2d.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_box2d.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_75b14332.o: ../../JuceLibraryCode/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_72d3da2c.o: ../../JuceLibraryCode/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_d2be882c.o: ../../JuceLibraryCode/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_9c18891e.o: ../../JuceLibraryCode/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_8a6da59c.o: ../../JuceLibraryCode/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_4a026f23.o: ../../JuceLibraryCode/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
