#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

//Sets up log category for this module
DECLARE_LOG_CATEGORY_EXTERN(LogToolbarEditor, All, All);
class FToolbarModule : public IModuleInterface
{
public:
    /**
     * @brief Startup is used to find where to place the toolbar and to start populating it.
     */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

    /**
     * @brief Function to create the toolbar menu.
     */
    void AddMenu(FMenuBarBuilder& MenuBarBuilder);
    /**
     * 
     * @brief Function to populate the menu dropdown with options 
     */
    void FillMenu(FMenuBuilder& MenuBuilder);
};
