//!
//! @file  PWDialMenuEventParameterType.h
//! @brief �_�C�������j���[�C�x���g�p�����[�^�^�C�v�w�b�_�[
//!
//! @author rinaba
//!
#pragma once

#include "PWApplicationLib/PWProcessUniqueKeyType.h"

namespace Planeswalker
{

class	PWDialMenuEventParameter;

extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuSelected);		//!< �_�C�������j���[���I�����ꂽ
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuClick);			//!< �_�C�������N���b�N����
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuRotate);			//!< �_�C��������]����
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuContactBegin);	//!< �_�C��������ʂɐڐG����
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuContactContinue);	//!< �_�C��������ʏ�œ�������
extern	PLANESWALKER_PROCESS_UNIQUE_KEY(kEventKindDialMenuContactEnd);		//!< �_�C��������ʂ��痣����

}
