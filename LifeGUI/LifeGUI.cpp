﻿/*
 *  GWEN
 *  Copyright (c) 2010 Facepunch Studios
 *  See license in Gwen.h
 */

#include "LifeGUI.h"
#include "Gwen/Controls/DockedTabControl.h"
#include "Gwen/Controls/WindowControl.h"
#include "Gwen/Controls/CollapsibleList.h"
#include "Gwen/Controls/Layout/Position.h"
#include "Gwen/Platform.h"

using namespace Gwen;

#define ADD_UNIT_TEST(name) \
    GUnit*RegisterLifeGUI_##name(Gwen::Controls::Base*tab); \
    { \
        Controls::Button* pButton = cat->Add( #name); \
        pButton->SetName( #name); \
        GUnit* test = RegisterLifeGUI_##name(pCenter); \
        test->Hide(); \
        test->SetLifeGUI(this); \
        pButton->onPress.Add(this, &ThisClass::OnCategorySelect, Gwen::Event::Packet(test)); \
    } \

Gwen::Controls::TabButton* pButton = NULL;

GWEN_CONTROL_CONSTRUCTOR(LifeGUI)
{
    m_pLastControl = NULL;
    Dock(Pos::Fill);
    SetSize(1024, 768);
    Controls::CollapsibleList* pList = new Controls::CollapsibleList(this);
    GetLeft()->GetTabControl()->AddPage("CollapsibleList", pList);
    GetLeft()->SetWidth(150);
    m_TextOutput = new Controls::ListBox(GetBottom());
    pButton = GetBottom()->GetTabControl()->AddPage("Output", m_TextOutput);
    GetBottom()->SetHeight(200);
    m_StatusBar = new Controls::StatusBar(this);
    m_StatusBar->Dock(Pos::Bottom);
    Controls::Layout::Center* pCenter = new Controls::Layout::Center(this);

    pCenter->Dock(Pos::Fill);
    {
        Controls::CollapsibleCategory* cat = pList->Add("Game of Life");
        GUnit*RegisterLifeGUI_LabelMultiline(Gwen::Controls::Base*tab);
        {
            Controls::Button* pButton = cat->Add("Simulator");
            pButton->SetName("Simulator");
            GUnit* test = RegisterLifeGUI_LabelMultiline(pCenter);
            test->Hide();
            test->SetLifeGUI(this);
            pButton->onPress.Add(this, &ThisClass::OnCategorySelect, Gwen::Event::Packet(test));
        }
    }
    {
        Controls::CollapsibleCategory* cat = pList->Add("Non-Interactive");
        ADD_UNIT_TEST(ProgressBar);
        ADD_UNIT_TEST(GroupBox);
        ADD_UNIT_TEST(ImagePanel);
        ADD_UNIT_TEST(StatusBar);
    }
    {
        Controls::CollapsibleCategory* cat = pList->Add("Controls");
        ADD_UNIT_TEST(ComboBox);
        ADD_UNIT_TEST(TextBox);
        ADD_UNIT_TEST(ListBox);
        ADD_UNIT_TEST(CrossSplitter);
        ADD_UNIT_TEST(RadioButton);
        ADD_UNIT_TEST(Checkbox);
        ADD_UNIT_TEST(Numeric);
        ADD_UNIT_TEST(Slider);
        ADD_UNIT_TEST(MenuStrip);
    }
    {
        Controls::CollapsibleCategory* cat = pList->Add("Containers");
        ADD_UNIT_TEST(Window);
        ADD_UNIT_TEST(TreeControl);
        ADD_UNIT_TEST(Properties);
        ADD_UNIT_TEST(TabControl);
        ADD_UNIT_TEST(ScrollControl);
        ADD_UNIT_TEST(PageControl);
    }
    {
        Controls::CollapsibleCategory* cat = pList->Add("Non-Standard");
        ADD_UNIT_TEST(CollapsibleList);
        ADD_UNIT_TEST(ColorPicker);
    }
    m_StatusBar->SendToBack();
    PrintText("Unit Test Started.");
    m_fLastSecond = Gwen::Platform::GetTimeInSeconds();
    m_iFrames = 0;
    pList->GetNamedChildren("MenuStrip").DoAction();

    PrintText(Utility::Format("Size of Button = %lu bytes.", sizeof(Gwen::Controls::Button)));
}

void LifeGUI::OnCategorySelect(Gwen::Event::Info info)
{
    if (m_pLastControl)
        m_pLastControl->Hide();

    info.Packet->Control->Show();
    m_pLastControl = info.Packet->Control;
}

void LifeGUI::PrintText(const Gwen::String& str)
{
    m_TextOutput->AddItem(str);
    m_TextOutput->ScrollToBottom();
}

void LifeGUI::Render(Gwen::Skin::Base* skin)
{
    m_iFrames++;

    if (m_fLastSecond < Gwen::Platform::GetTimeInSeconds())
    {
        m_StatusBar->SetText(Gwen::Utility::Format("GWEN Unit Test - %i fps", m_iFrames*2));
        m_fLastSecond = Gwen::Platform::GetTimeInSeconds()+0.5f;
        m_iFrames = 0;
    }

    BaseClass::Render(skin);
}

void GUnit::UnitPrint(Gwen::String str)
{
    m_pLifeGUI->PrintText(str);
}