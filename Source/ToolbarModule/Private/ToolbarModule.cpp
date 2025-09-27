#include "ToolbarModule.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FToolbarModule"
DEFINE_LOG_CATEGORY(LogToolbarEditor)

void FToolbarModule::StartupModule()
{
	//get Level Editor module (basically the whole editor) so we can add to its toolbar
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	//Creating a multibox menu extender object to handle the "extend" on Level Editor's toolbar
	const TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());

	MenuExtender->AddMenuBarExtension(
	"Help", //Where to "hook" this extension. to "Help" on the main toolbar
	EExtensionHook::After, //which side of "Help" to hook on to
	nullptr, //optional FUICommandList. I.e., keyboard combination to shortcut the option

	//delegate the process of building the menu bar to another function.
	//Function will take FMenuBarBuilder& as argument
	FMenuBarExtensionDelegate::CreateRaw(this, &FToolbarModule::AddMenu)  
	);

	//Gets the manager that handles adding new menus, and adds our previous lines
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FToolbarModule::ShutdownModule()
{
    
}

void FToolbarModule::AddMenu(FMenuBarBuilder& MenuBarBuilder)
{
	MenuBarBuilder.AddPullDownMenu(
		FText::FromString("IdolOnDuty"), //name of Toolbar Menu
		FText::FromString("Idol On Duty Editor Tools"), //tooltip
		//function to call to fill in the dropdown menu, takes argument &FMenuBuilder
		FNewMenuDelegate::CreateRaw(this, &FToolbarModule::FillMenu)
		);
}

void FToolbarModule::FillMenu(FMenuBuilder& MenuBuilder)
{
	//Adding a search bar and seperator
	MenuBuilder.AddSearchWidget();
	MenuBuilder.AddMenuSeparator();
	//An example menu button to show functionality.
	//This will print a log message after clicking it
	MenuBuilder.AddMenuEntry(
		FText::FromString("Example"), //Name of button
		FText::FromString("Example Log Message"), //Tooltip
		//The icon to show next to the button. Information gathered from SlateEditorStyle.cpp in Engine
		FSlateIcon("Icons", "Info"),
		//delegated function to handle what the button does. In this case, a lambda function instead of calling outside
		FUIAction(FExecuteAction::CreateLambda([]()
		{
			UE_LOG(LogToolbarEditor, Display, TEXT("Example Log Message Sent!"));
			
		}))
		);

	MenuBuilder.AddSubMenu(
		FText::FromString("Example SubMenu"),
		FText::FromString("Example of creating a SubMenu"),
		FNewMenuDelegate::CreateLambda([](FMenuBuilder& SubMenuBuilder)
		{

		SubMenuBuilder.AddMenuEntry(
		FText::FromString("Sub Example"),
				FText::FromString("Example of Sub Menu Button"),
				FSlateIcon("Icons", "Error"),
				FUIAction(FExecuteAction::CreateLambda([]()
				{
					UE_LOG(LogToolbarEditor, Display, TEXT("SubMenu Log Message Sent!"));
			
				}))
				);
		})
	
		);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FToolbarModule, ToolbarModule)