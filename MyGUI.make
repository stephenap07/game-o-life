ifndef verbose
  SILENT = @
endif

EXE=life
CC=g++
CXX=g++
INCLUDES=-IGWork/gwen/include/ -Iinclude/
CFLAGS += -msse -O2 -ffast-math
CXXFLAGS=${INCLUDES} -msse -O2 -ffast-math
LINKCMD=
OBJDIR=obj
OBJECTS := \
	$(OBJDIR)/Button.o \
	$(OBJDIR)/Checkbox.o \
	$(OBJDIR)/CollapsibleList.o \
	$(OBJDIR)/ColorPicker.o \
	$(OBJDIR)/ComboBox.o \
	$(OBJDIR)/CrossSplitter.o \
	$(OBJDIR)/GroupBox.o \
	$(OBJDIR)/ImagePanel.o \
	$(OBJDIR)/Label.o \
	$(OBJDIR)/LabelMultiline.o \
	$(OBJDIR)/ListBox.o \
	$(OBJDIR)/MenuStrip.o \
	$(OBJDIR)/Numeric.o \
	$(OBJDIR)/PageControl.o \
	$(OBJDIR)/ProgressBar.o \
	$(OBJDIR)/Properties.o \
	$(OBJDIR)/RadioButton.o \
	$(OBJDIR)/ScrollControl.o \
	$(OBJDIR)/Slider.o \
	$(OBJDIR)/StatusBar.o \
	$(OBJDIR)/TabControl.o \
	$(OBJDIR)/TextBox.o \
	$(OBJDIR)/TreeControl.o \
	$(OBJDIR)/UnitTest.o \
	$(OBJDIR)/Window.o \

.PHONY: all debug run gui

all: ${OBJECTS}
	ar -rcs libmygui.a $(OBJECTS)

run:
	./$(EXE)

$(OBJDIR)/Button.o: UnitTest/Button.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Checkbox.o: UnitTest/Checkbox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/CollapsibleList.o: UnitTest/CollapsibleList.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ColorPicker.o: UnitTest/ColorPicker.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ComboBox.o: UnitTest/ComboBox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/CrossSplitter.o: UnitTest/CrossSplitter.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/GroupBox.o: UnitTest/GroupBox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ImagePanel.o: UnitTest/ImagePanel.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Label.o: UnitTest/Label.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/LabelMultiline.o: UnitTest/LabelMultiline.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ListBox.o: UnitTest/ListBox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/MenuStrip.o: UnitTest/MenuStrip.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Numeric.o: UnitTest/Numeric.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/PageControl.o: UnitTest/PageControl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ProgressBar.o: UnitTest/ProgressBar.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Properties.o: UnitTest/Properties.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/RadioButton.o: UnitTest/RadioButton.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ScrollControl.o: UnitTest/ScrollControl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Slider.o: UnitTest/Slider.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/StatusBar.o: UnitTest/StatusBar.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/TabControl.o: UnitTest/TabControl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/TextBox.o: UnitTest/TextBox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/TreeControl.o: UnitTest/TreeControl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/UnitTest.o: UnitTest/UnitTest.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Window.o: UnitTest/Window.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
