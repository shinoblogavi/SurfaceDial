//!
//! @file  URDialMenu.cpp
//! @brief SurfaceDialのメニューの設定
//! @date 2017/02/14
//! @author S.Yuasa
//!
#include "Urza/URDialMenu.h"
#include "Urza/URGrade.h"
#include "Urza/URApplicationFilePathType.h"
#include "Urza/URResource.h"
#include "Urza/ResourceID/kGroupIDDialMenu.h"
#include "PWKernelLib/PWFilePath.h"
#include "PWApplicationLib/PWProcessUniqueKey.h"
#include "PWApplicationLib/PWDialMenu.h"
#include "PWApplicationLib/PWApplicationFilePath.h"

namespace Planeswalker
{

namespace Urza
{

// ダイアルメニュー用のID
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemCanvasZoom);		// 表示倍率
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemCanvasRotate);		// 表示回転
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemBrushSize);			// ブラシサイズ
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemTimeLine);			// タイムライン
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemUndo);				// 取り消し
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemParallelRuler);		// 平行線定規

//!
//! @brief SurfaceDialのメニューを作成
//!
//! @param menuItems	[out] Dialに設定するメニューの配列
//!
void URDialMenu::InitializeMenu(std::vector<PWDialMenuItem>& menuItems)
{
	PWResourceRef resource = URResource::GetResource();
	PWString menuName, iconFolderPath, iconFilePath;
	PWApplicationFilePath::GetFolderPath(kApplicationFolderPathKindInstallDialIcon, iconFolderPath);

	// 表示倍率
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "zoom.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuCanvasZoom, resource);
	PWDialMenuItem zoomMenu(kURDialMenuItemCanvasZoom, menuName, iconFilePath);
	zoomMenu.SetRotation(1);
	zoomMenu.SetFeedback(FALSE);
	menuItems.push_back(zoomMenu);

	// 表示回転
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "rotation.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuCanvasRotate, resource);
	PWDialMenuItem rotateMenu(kURDialMenuItemCanvasRotate, menuName, iconFilePath);
	rotateMenu.SetRotation(1);
	rotateMenu.SetFeedback(FALSE);
	menuItems.push_back(rotateMenu);

	// 取り消し
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "undo.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuUndo, resource);
	PWDialMenuItem undoMenu(kURDialMenuItemUndo, menuName, iconFilePath);
	undoMenu.SetRotation(10);
	undoMenu.SetFeedback(TRUE);
	menuItems.push_back(undoMenu);

	// ブラシサイズ
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "brushsize.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuBrushSize, resource);
	PWDialMenuItem brushSizeMenu(kURDialMenuItemBrushSize, menuName, iconFilePath);
	brushSizeMenu.SetRotation(1);
	brushSizeMenu.SetFeedback(FALSE);
	menuItems.push_back(brushSizeMenu);

	// タイムライン
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "timeline.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuTimeLine, resource);
	PWDialMenuItem timelineMenu(kURDialMenuItemTimeLine, menuName, iconFilePath);
	timelineMenu.SetRotation(1);
	timelineMenu.SetFeedback(FALSE);
	menuItems.push_back(timelineMenu);

	if (!CheckIsDebutSerial) {
		// 平行線定規
		iconFilePath = iconFolderPath;
		PWFilePath::AddLastPath(iconFilePath, "parallelruler.png");
		PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuParallelRuler, resource);
		PWDialMenuItem parallelRulerMenu(kURDialMenuItemParallelRuler, menuName, iconFilePath);
		parallelRulerMenu.SetRotation(1);
		parallelRulerMenu.SetFeedback(FALSE);
		menuItems.push_back(parallelRulerMenu);
	}
}

}

}
