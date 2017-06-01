//!
//! @file  PWDialMenuEventParameter.cpp
//! @brief �_�C�������j���[�C�x���g�p�����[�^�N���X�\�[�X
//!
//! @author rinaba
//!
#include "PWApplicationLib/PWDialMenuEventParameter.h"
#include "PWApplicationLib/PWProcessUniqueKey.h"
#include "PWApplicationLib/PWEvent.h"

namespace Planeswalker
{
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuSelected);			//!< �_�C�������j���[���I�����ꂽ
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuClick);				//!< �_�C�������N���b�N����
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuRotate);				//!< �_�C��������]����
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuContactBegin);		//!< �_�C��������ʂɐڐG����
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuContactContinue);	//!< �_�C��������ʏ�œ�������
		PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kEventKindDialMenuContactEnd);			//!< �_�C��������ʂ��痣����

static	PLANESWALKER_PROCESS_UNIQUE_KEY_NAMELESS(kDialMenuEventParameterKeyParameter);	//!< �_�C�������j���[�p�����[�^.

//-----------------------------
//!
//! @brief �R���X�g���N�^.
//! 
//-----------------------------
PWDialMenuEventParameter::PWDialMenuEventParameter()
{
	InitializeMembers();
}

//-----------------------------
//!
//! @brief	�����t�R���X�g���N�^(�C�x���g���).
//! @param	dialMenuEventKind	[in]�C�x���g���.
//! 
//-----------------------------
PWDialMenuEventParameter::PWDialMenuEventParameter( const EventKind dialMenuEventKind )
{
	InitializeMembers();
	SetDialMenuEventKind(dialMenuEventKind);
}

//-----------------------------
//!
//! @brief	�����t�R���X�g���N�^(�C�x���g).
//! @param	rEvent	[in]�Z�b�g����C�x���g.
//! 
//-----------------------------
PWDialMenuEventParameter::PWDialMenuEventParameter( const PWEvent& rEvent )
{
	InitializeMembers();
	SetEvent(rEvent);
}

//-----------------------------
//!
//! @brief �R�s�[�R���X�g���N�^.
//! 
//-----------------------------
PWDialMenuEventParameter::PWDialMenuEventParameter( const PWDialMenuEventParameter& rDialMenuEventParameter )
{
	InitializeMembers();
	*this	= rDialMenuEventParameter;
}

//-----------------------------
//!
//! @brief �f�B�X�g���N�^.
//! 
//-----------------------------
PWDialMenuEventParameter::~PWDialMenuEventParameter()
{
}

//-----------------------------
//!
//! @brief �����o�[�ϐ��̏�����.
//! 
//-----------------------------
void PWDialMenuEventParameter::InitializeMembers()
{
	m_dialMenuEventKind = 0;

	SDialMenuEventParameter parameter = {};
	m_parameter = parameter;
}

//-----------------------------
//!
//! @brief	�A�C�h���C�x���g��.
//! @param	eventKind	[in]�C�x���g���.
//! 
//-----------------------------
Bool PWDialMenuEventParameter::IsDialMenuEventKind( const EventKind eventKind )
{
	return
		eventKind == kEventKindDialMenuSelected ||
		eventKind == kEventKindDialMenuClick ||
		eventKind == kEventKindDialMenuRotate ||
		eventKind == kEventKindDialMenuContactBegin ||
		eventKind == kEventKindDialMenuContactContinue ||
		eventKind == kEventKindDialMenuContactEnd;
}

//-----------------------------
//!
//! @brief	�_�C�������j���[�C�x���g�̎�ނ�ݒ�.
//!	@param	dialMenuEventKind	[in]	�_�C�������j���[�C�x���g�̎��.
//! 
//-----------------------------
void PWDialMenuEventParameter::SetDialMenuEventKind( const EventKind dialMenuEventKind )
{
	if( IsDialMenuEventKind(dialMenuEventKind) )
	{
		m_dialMenuEventKind	= dialMenuEventKind;
	}
}

//-----------------------------
//!
//! @brief	�_�C�������j���[�C�x���g�̎�ނ��擾.
//! @return	�_�C�������j���[�C�x���g�̎��.
//! 
//-----------------------------
UInt32 PWDialMenuEventParameter::GetDialMenuEventKind() const
{
	return m_dialMenuEventKind;
}

//!
//!	@brief	�_�C�������j���[ID�̐ݒ�
//!
void PWDialMenuEventParameter::SetMenuID( const UInt32 menuID )
{
	m_parameter.menuID = menuID;
}

//!
//!	@brief	�_�C�������j���[ID�̎擾
//!
UInt32 PWDialMenuEventParameter::GetMenuID() const
{
	return m_parameter.menuID;
}

//!
//!	@brief	��]�p�x�ݒ�
//!
void PWDialMenuEventParameter::SetRotatin( const double degree )
{
	m_parameter.rotation = degree;
}

//!
//!	@brief	��]�p�x�擾
//!
double PWDialMenuEventParameter::GetRoation() const
{
	return m_parameter.rotation;
}

//!
//!	@brief	��ʂɐڐG���Ă邩�ݒ�
//!
void PWDialMenuEventParameter::SetContact( const Bool contact )
{
	m_parameter.contact = contact;
}

//!
//!	@brief	��ʂɐڐG���Ă邩�擾
//!
Bool PWDialMenuEventParameter::IsContact() const
{
	return m_parameter.contact;
}

//!
//!	@brief	��ʂɐڐG���Ă�̈�̐ݒ�
//!
void PWDialMenuEventParameter::SetBounds( const SRect& bounds )
{
	m_parameter.bounds = bounds;
}

//!
//!	@brief	��ʂɐڐG���Ă�̈�̎擾
//!
SRect PWDialMenuEventParameter::GetBounds() const
{
	return m_parameter.bounds;
}

//!
//!	@brief	��ʂɐڐG���Ă�ʒu�̐ݒ�
//!
void PWDialMenuEventParameter::SetPosition( const SPoint& position )
{
	m_parameter.position = position;
}

//!
//!	@brief	��ʂɐڐG���Ă���ʒu���擾
//!
SPoint PWDialMenuEventParameter::GetPosition() const
{
	return m_parameter.position;
}

//-----------------------------
//!
//! @brief	�C�x���g�̐ݒ�.
//! @param	rEvent	[in]�Z�b�g����C�x���g.
//! 
//-----------------------------
void PWDialMenuEventParameter::SetEvent( const PWEvent& rEvent )
{
	InitializeMembers();
	if( IsDialMenuEventKind(rEvent.GetEventKind()) )
	{
		m_dialMenuEventKind	= rEvent.GetEventKind();
		rEvent.GetValue(kDialMenuEventParameterKeyParameter,m_parameter);
	}
}

//-----------------------------
//!
//! @brief	�C�x���g�̎擾.
//! @param	rEvent	[out]�擾�����C�x���g.
//! 
//-----------------------------
void PWDialMenuEventParameter::GetEvent( PWEvent& rEvent )
{
	rEvent.SetEventKind(m_dialMenuEventKind);
	rEvent.SetValue(kDialMenuEventParameterKeyParameter,m_parameter);
}

}
