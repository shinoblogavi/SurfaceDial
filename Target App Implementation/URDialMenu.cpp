//!
//! @file  URDialMenu.cpp
//! @brief SurfaceDial�̃��j���[�̐ݒ�
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

// �_�C�A�����j���[�p��ID
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemCanvasZoom);		// �\���{��
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemCanvasRotate);		// �\����]
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemBrushSize);			// �u���V�T�C�Y
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemTimeLine);			// �^�C�����C��
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemUndo);				// ������
PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kURDialMenuItemParallelRuler);		// ���s����K

//!
//! @brief SurfaceDial�̃��j���[���쐬
//!
//! @param menuItems	[out] Dial�ɐݒ肷�郁�j���[�̔z��
//!
void URDialMenu::InitializeMenu(std::vector<PWDialMenuItem>& menuItems)
{
	PWResourceRef resource = URResource::GetResource();
	PWString menuName, iconFolderPath, iconFilePath;
	PWApplicationFilePath::GetFolderPath(kApplicationFolderPathKindInstallDialIcon, iconFolderPath);

	// �\���{��
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "zoom.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuCanvasZoom, resource);
	PWDialMenuItem zoomMenu(kURDialMenuItemCanvasZoom, menuName, iconFilePath);
	zoomMenu.SetRotation(1);
	zoomMenu.SetFeedback(FALSE);
	menuItems.push_back(zoomMenu);

	// �\����]
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "rotation.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuCanvasRotate, resource);
	PWDialMenuItem rotateMenu(kURDialMenuItemCanvasRotate, menuName, iconFilePath);
	rotateMenu.SetRotation(1);
	rotateMenu.SetFeedback(FALSE);
	menuItems.push_back(rotateMenu);

	// ������
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "undo.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuUndo, resource);
	PWDialMenuItem undoMenu(kURDialMenuItemUndo, menuName, iconFilePath);
	undoMenu.SetRotation(10);
	undoMenu.SetFeedback(TRUE);
	menuItems.push_back(undoMenu);

	// �u���V�T�C�Y
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "brushsize.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuBrushSize, resource);
	PWDialMenuItem brushSizeMenu(kURDialMenuItemBrushSize, menuName, iconFilePath);
	brushSizeMenu.SetRotation(1);
	brushSizeMenu.SetFeedback(FALSE);
	menuItems.push_back(brushSizeMenu);

	// �^�C�����C��
	iconFilePath = iconFolderPath;
	PWFilePath::AddLastPath(iconFilePath, "timeline.png");
	PWResource::LoadString(menuName, kGroupIDDialMenu::kStringIDMenuTimeLine, resource);
	PWDialMenuItem timelineMenu(kURDialMenuItemTimeLine, menuName, iconFilePath);
	timelineMenu.SetRotation(1);
	timelineMenu.SetFeedback(FALSE);
	menuItems.push_back(timelineMenu);

	if (!CheckIsDebutSerial) {
		// ���s����K
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
