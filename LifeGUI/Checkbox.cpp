#include "LifeGUI.h"
#include "Gwen/Controls/CheckBox.h"

using namespace Gwen;

class Checkbox : public GUnit
{
public:

    GWEN_CONTROL_INLINE(Checkbox, GUnit)
    {
        Gwen::Controls::CheckBox* check = new Gwen::Controls::CheckBox(this);
        check->SetPos(10, 10);
        check->onChecked.Add(this, &Checkbox::OnChecked);
        check->onUnChecked.Add(this, &Checkbox::OnUnchecked);
        check->onCheckChanged.Add(this, &Checkbox::OnCheckChanged);
        Gwen::Controls::CheckBoxWithLabel* labeled = new Gwen::Controls::CheckBoxWithLabel(this);
        labeled->SetPos(10, 30);
        labeled->Label()->SetText("Labeled CheckBox");
        labeled->Checkbox()->onChecked.Add(this, &Checkbox::OnChecked);
        labeled->Checkbox()->onUnChecked.Add(this, &Checkbox::OnUnchecked);
        labeled->Checkbox()->onCheckChanged.Add(this, &Checkbox::OnCheckChanged);
        {
            Gwen::Controls::CheckBox* check = new Gwen::Controls::CheckBox(this);
            check->SetPos(10, 54);
            check->SetDisabled(true);
        }
    }

    void OnChecked(Controls::Base* pControl)
    {
        UnitPrint("Checkbox Checked (using 'OnChecked' event)");
    }

    void OnUnchecked(Controls::Base* pControl)
    {
        UnitPrint("Checkbox Unchecked (using 'OnUnchecked' event)");
    }

    void OnCheckChanged(Controls::Base* pControl)
    {
        UnitPrint("Checkbox CheckChanged (using 'OnCheckChanged' event)");
    }

};


DEFINE_UNIT_TEST(Checkbox, "Checkbox");