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
	$(OBJDIR)/LifeGUI.o \
	$(OBJDIR)/Window.o \

.PHONY: all debug run gui

all: ${OBJECTS}
	ar -rcs liblifegui.a $(OBJECTS)

run:
	./$(EXE)

$(OBJDIR)/Button.o: LifeGUI/Button.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Checkbox.o: LifeGUI/Checkbox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/CollapsibleList.o: LifeGUI/CollapsibleList.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ColorPicker.o: LifeGUI/ColorPicker.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ComboBox.o: LifeGUI/ComboBox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/CrossSplitter.o: LifeGUI/CrossSplitter.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/GroupBox.o: LifeGUI/GroupBox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ImagePanel.o: LifeGUI/ImagePanel.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Label.o: LifeGUI/Label.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/LabelMultiline.o: LifeGUI/LabelMultiline.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ListBox.o: LifeGUI/ListBox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/MenuStrip.o: LifeGUI/MenuStrip.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Numeric.o: LifeGUI/Numeric.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/PageControl.o: LifeGUI/PageControl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ProgressBar.o: LifeGUI/ProgressBar.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Properties.o: LifeGUI/Properties.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/RadioButton.o: LifeGUI/RadioButton.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/ScrollControl.o: LifeGUI/ScrollControl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Slider.o: LifeGUI/Slider.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/StatusBar.o: LifeGUI/StatusBar.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/TabControl.o: LifeGUI/TabControl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/TextBox.o: LifeGUI/TextBox.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/TreeControl.o: LifeGUI/TreeControl.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/LifeGUI.o: LifeGUI/LifeGUI.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
$(OBJDIR)/Window.o: LifeGUI/Window.cpp
	@echo $(notdir $<)
	$(SILENT) $(CXX) $(CXXFLAGS) -o "$@" -c "$<"
