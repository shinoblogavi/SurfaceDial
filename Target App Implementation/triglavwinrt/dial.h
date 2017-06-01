//!
//!	@brief	�T�[�t�F�X�_�C�������C�u����.
//!
//!	@note	Windows::UI::Input::RadialController�ɑΉ�����֐����������Ă��܂�.
//!	@note	�ڍׂȐU�镑����msdn���Q�Ƃ��Ă�������.
//!	@note	https://docs.microsoft.com/en-us/uwp/api/windows.ui.input.radialcontroller
//!
//!	@note	���ׂĂ̊֐���dial_result��Ԃ��܂�.
//!	@note	�֐��̌��ʂ�out�����Ŏ󂯎��܂�. ���s�����ꍇout�����ɕύX�������܂���.
//!
#pragma once

#include <stdint.h>

#ifndef TRIGLAV_WINRT_API
#define TRIGLAV_WINRT_API __declspec(dllimport)
#endif
#define TRIGLAV_WINRT_CALL __cdecl

extern "C" {

//!	@brief	�֐��̌Ăяo������.
typedef int32_t	dial_result_t;
static const dial_result_t	dial_ok = 0;		//!< ����.
static const dial_result_t	dial_failed = 1;	//!< ���s.

//!	@brief	int
typedef int32_t	dial_int_t;

//!	@breif	bool
typedef int32_t	dial_bool_t;
static const dial_bool_t	dial_false = 0;		//!< ����.
static const dial_bool_t	dial_true = 1;		//!< ���s.

//!	@brief	double
typedef double	dial_double_t;

//!	@brief	�ėp�n���h���^
typedef void *	dial_handle_t;

//!	@breif	�_�C�����R���g���[��.
typedef struct _dial_controller		dial_controller_t;

//!	@brief	�_�C�������j���[�A�C�e��.
typedef struct _dial_menu_item		dial_menu_item_t;

//!	@brief	�_�C�����t�@�C��.
typedef struct _dial_file			dial_file_t;

//!	@biref	�_�C�����R���^�N�g.(�T�[�t�F�X�_�C�����̈ʒu�Ɣ͈͂�\��).
typedef struct _dial_contact		dial_contact_t;

//!	@brief	Rect
typedef struct _dial_rect			dial_rect_t;

//!	@brief	Point
typedef struct _dial_point			dial_point_t;

//!
//!	@brief	dial�̏�����. �ق���dial�֐����Ăяo���O�ɕK���Ăяo���Ă�������.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_initialize	();

//!
//!	@brief	dial�̏I������. �Ăяo����A�ق��̂�����dial�֐����Ăяo���Ă͂����܂���.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_uninitialize();

//!
//!	@brief	�T�[�t�F�X�_�C�������T�|�[�g���Ă��邩.
//!
//!	@param	
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_is_supported			( dial_bool_t *supported );

//!
//!	@brief	�T�[�t�F�X�_�C�����R���g���[���[�̍쐬. �g���I�������dial_controller_release�Ŕj�����Ă�������.
//!
//!	@param	hwnd		[in]�Ώۂ̃E�C���h�E
//!	@param	userdata	[in]�Ăяo�����Ŏ��R�Ɏg�p. get_userdata�Ŏ擾�ł���.
//!	@param	controller	[out]�쐬���ꂽ�R���g���[���[.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_create		( dial_handle_t hwnd, void *userdata, dial_controller_t **controller );

//!
//!	@brief	�T�[�t�F�X�_�C�����R���g���[���[�̔j��
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_release		( dial_controller_t *controller );

//!
//!	@brief	���[�U�[�f�[�^�̎擾.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	userdata	[out]dial_controller_create�Ŏw�肵�����[�U�[�f�[�^.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_get_userdata	( dial_controller_t *controller, void **userdata );

//!
//!	@brief	��]�p�x�̎擾
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	degree		[out]��]�p�x.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_get_rotation	( dial_controller_t *controller, dial_double_t *degree );

//!
//!	@brief	��]�p�x�̐ݒ�. ���̊p�x���Ɏw�肵��dial_controller_rotation_changed_f���Ăяo�����.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	degree		[in]��]�p�x.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_set_rotation	( dial_controller_t *controller, dial_double_t degree );

//!
//!	@brief	��]�̃t�B�[�h�o�b�N�͗L����.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	enabled		[out]dial_true�L��, dial_false����
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_is_feedback	( dial_controller_t *controller, dial_bool_t *enabled );

//!
//!	@brief	��]�̃t�B�[�h�o�b�N�̐ݒ�.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	enabled		[in]dial_true�L��, dial_false����
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_set_feedback	( dial_controller_t *controller, dial_bool_t enabled );

//!	@brief	�V�X�e�����j���[�A�C�e��.(����J)
typedef enum dial_default_menu_item dial_default_menu_item_t;

//!
//!	@brief	�V�X�e�����j���[�̐ݒ�.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	items		[in]�ݒ肷��V�X�e�����j���[�̔z��
//!	@param	item_count	[in]items�̐�
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_configuration_set_default_menu_items			( dial_controller_t *controller, dial_default_menu_item_t *items, dial_int_t item_count );

//!
//!	@brief	�V�X�e�����j���[�̑I�������݂�
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[
//!	@param	item		[in]�I��Ώ�
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_configuration_try_select_default_menu_item	( dial_controller_t *controller, dial_default_menu_item_t item );

//!
//!	@brief	���j���[�����Z�b�g����.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_configuration_reset_to_default_menu_items	( dial_controller_t *controller );

//!
//!	@brief	�N���b�N�̃R�[���o�b�N�֐�.
//!
//!	@param	controller	[in]�Ăяo�����̃R���g���[��
//!	@param	contact		[in]�R���^�N�g
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_clicked_f)			(dial_controller_t *controller, const dial_contact_t *contact);

//!
//!	@brief	�R���g���[���̊l���̃R�[���o�b�N�֐�.
//!
//!	@param	controller	[in]�Ăяo�����̃R���g���[��
//!	@param	contact		[in]�R���^�N�g
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_acquired_f)			(dial_controller_t *controller, const dial_contact_t *contact);

//!
//!	@brief	�R���g���[���̑r���R�[���o�b�N�֐�.
//!
//!	@param	controller	[in]�Ăяo�����̃R���g���[��
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_control_lost_f)		(dial_controller_t *controller);

//!
//!	@brief	��]�̃R�[���o�b�N�֐�.
//!
//!	@param	controller	[in]�Ăяo�����̃R���g���[��
//!	@param	contact		[in]�R���^�N�g
//!	@param	delta		[in]��]��(��)
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_rotation_changed_f)	(dial_controller_t *controller, const dial_contact_t *contact, const dial_double_t delta);

//!
//!	@brief	�ڐG�p���̃R�[���o�b�N�֐�
//!
//!	@param	controller	[in]�Ăяo�����̃R���g���[��
//!	@param	contact		[in]�R���^�N�g
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_contact_continue_f)	(dial_controller_t *controller, const dial_contact_t *contact);

//!
//!	@brief	�ڐG�I���̃R�[���o�b�N�֐�
//!
//!	@param	controller	[in]�Ăяo�����̃R���g���[��
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_contact_ended_f)		(dial_controller_t *controller);

//!
//!	@brief	�ڐG�J�n�̃R�[���o�b�N�֐�
//!
//!	@param	controller	[in]�Ăяo�����̃R���g���[��
//!	@param	contact		[in]�R���^�N�g
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_contact_started_f)	(dial_controller_t *controller, const dial_contact_t *contact);

//!
//!	@brief	�N���b�N�R�[���o�b�N�֐��̒ǉ�.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�R�[���o�b�N�֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_clicked			( dial_controller_t *controller, dial_controller_clicked_f f );

//!
//!	@brief	�l���R�[���o�b�N�֐��̒ǉ�.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�R�[���o�b�N�֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_acquired			( dial_controller_t *controller, dial_controller_acquired_f f );

//!
//!	@brief	�r���R�[���o�b�N�֐��̒ǉ�.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�R�[���o�b�N�֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_control_lost		( dial_controller_t *controller, dial_controller_control_lost_f f );

//!
//!	@brief	��]�R�[���o�b�N�֐��̒ǉ�.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�R�[���o�b�N�֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_rotation_changed	( dial_controller_t *controller, dial_controller_rotation_changed_f f );

//!
//!	@brief	�ڐG�p���R�[���o�b�N�֐��̒ǉ�.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�R�[���o�b�N�֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_contact_continue	( dial_controller_t *controller, dial_controller_contact_continue_f f );

//!
//!	@brief	�ڐG�I���R�[���o�b�N�֐��̒ǉ�.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�R�[���o�b�N�֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_contact_ended	( dial_controller_t *controller, dial_controller_contact_ended_f f );

//!
//!	@brief	�ڐG�J�n�R�[���o�b�N�֐��̒ǉ�.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�R�[���o�b�N�֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_contact_started	( dial_controller_t *controller, dial_controller_contact_started_f f );

//!
//!	@brief	�N���b�N�R�[���o�b�N�֐��̍폜.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�Ώۂ̊֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_clicked			( dial_controller_t *controller, dial_controller_clicked_f f );

//!
//!	@brief	�l���R�[���o�b�N�֐��̍폜.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�Ώۂ̊֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_acquired			( dial_controller_t *controller, dial_controller_acquired_f f );

//!
//!	@brief	�r���R�[���o�b�N�֐��̍폜.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�Ώۂ̊֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_control_lost		( dial_controller_t *controller, dial_controller_control_lost_f f );

//!
//!	@brief	��]�R�[���o�b�N�֐��̍폜.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�Ώۂ̊֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_rotation_changed	( dial_controller_t *controller, dial_controller_rotation_changed_f f );

//!
//!	@brief	�ڐG�p���R�[���o�b�N�֐��̍폜.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�Ώۂ̊֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_contact_continue	( dial_controller_t *controller, dial_controller_contact_continue_f f );

//!
//!	@brief	�ڐG�I���R�[���o�b�N�֐��̍폜.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�Ώۂ̊֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_contact_ended		( dial_controller_t *controller, dial_controller_contact_ended_f f );

//!
//!	@brief	�ڐG�J�n�R�[���o�b�N�֐��̍폜.
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[.
//!	@param	f			[in]�Ώۂ̊֐�.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_contact_started	( dial_controller_t *controller, dial_controller_contact_started_f f );

//!
//!	@brief	���j���[�A�C�e����ݒ肷��
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[
//!	@param	items		[in]�A�C�e���̔z��
//!	@param	item_count	[in]items�̐�
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_set_items				( dial_controller_t *controller, dial_menu_item_t **items, dial_int_t item_count );

//!
//!	@brief	�I����Ԃ̃A�C�e�����擾
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[
//!	@param	item		[out]�I����Ԃ̃A�C�e��. �V�X�e�����j���[��I������Ă��Ȃ��Ƃ�NULL
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_get_selected_menu_item	( dial_controller_t *controller, dial_menu_item_t **item );

//!
//!	@brief	�I����Ԃ̃A�C�e����ݒ�
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[
//!	@param	item		[in]�I������A�C�e��
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_set_selected_menu_item	( dial_controller_t *controller, dial_menu_item_t *items );

//!
//!	@brief	�O��I�����ꂽ�A�C�e���̑I�������݂�
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_try_select_previous		( dial_controller_t *controller );

//!
//!	@brief	���j���[���L����
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[
//!	@param	enabled		[out]dial_true�L��, dial_false����
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_is_enabled				( dial_controller_t *controller, dial_bool_t *enabled );

//!
//!	@brief	���j���[�̗L��������ݒ�
//!
//!	@param	controller	[in]�Ώۂ̃R���g���[���[
//!	@param	enabled		[in]dial_true�L��, dial_false����
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_set_enabled				( dial_controller_t *controller, dial_bool_t enabled );

//!
//!	@brief	�t�@�C���쐬�R�[���o�b�N�֐�
//!
//!	@param	file	[in]�쐬���ꂽ�t�@�C��. ���s�����Ƃ�NULL. dial_file_release�Ŕj�����邱��.
//!	@param	result	[in]����
//!
typedef void (TRIGLAV_WINRT_CALL* dial_file_created_f)( dial_file_t *file, dial_result_t result );

//!
//!	@brief	�t�@�C���̍쐬. ���ʂ̓R�[���o�b�N�Ŏ󂯎��܂�.
//!
//!	@param	path	[in]�t�@�C���p�X
//!	@param	userdata[in]�Ăяo�������g�p�ł��郆�[�U�[�f�[�^
//!	@param	f		[in]�t�@�C���쐬�֐�
//!
//!	@return	dial_reuslt. dial_ok��Ԃ��Ƃ��R�[���o�b�N�͌Ăяo����܂�.
//!
//!	@note	�R�[���o�b�N���Ăяo�����X���b�h��dial_initialize�̏��������@�ɂ���ĈقȂ�܂�.(���Ԃ�)
//!	@note	�V���O���X���b�h�ŏ����������ꍇ�̓��C���X���b�h�A�}���`�X���b�h�ŏ����������ꍇ�̓��[�J�[�X���b�h�ŃR�[���o�b�N�����s����܂�.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_file_create			( const wchar_t *path, void *userdata, dial_file_created_f f );

//!
//!	@brief	�t�@�C���̔j��
//!
//!	@param	file	[in]�t�@�C��
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_file_release		( dial_file_t *file );

//!
//!	@brief	�t�@�C���p�X�̎擾
//!
//!	@param	file	[in]�t�@�C��
//!	@param	path	[out]�t�@�C���p�X
//!	@param	len		[out]path�̕�����
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_file_get_path		( dial_file_t *file, const wchar_t **path, dial_int_t *len );

//!
//!	@brief	���[�U�[�f�[�^�̎擾
//!
//!	@param	file	[in]�t�@�C��
//!	@param	userdata[out]���[�U�[�f�[�^
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_file_get_userdata	( dial_file_t *file, void **userdata );

//!
//!	@brief	�V�X�e���A�C�R��.
//!
typedef enum {
	dial_icon_scroll = 1,
	dial_icon_zoom = 2,
	dial_icon_undo_redo = 3,
	dial_icon_volume = 4,
	dial_icon_next_previous_track = 5,
	dial_icon_ruler = 6,
	dial_icon_ink_color = 7,
	dial_icon_ink_thickness = 8,
	dial_icon_pen_type = 9,
} dial_icon_t;

//!
//!	@brief	���j���[�A�C�e���̍쐬. dial_menu_item_release�Ŕj�����Ă�������.
//!
//!	@param	name	[in]�\�����O
//!	@param	icon	[in]�\���A�C�R��
//!	@param	userdata[in]�Ăяo�����ŗ��p���郆�[�U�[�f�[�^
//!	@param	item	[out]�쐬���ꂽ���j���[�A�C�e��
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_create			( const wchar_t *name, const dial_icon_t icon, void *userdata, dial_menu_item_t **item );

//!
//!	@brief	���j���[�A�C�e���̍쐬. dial_menu_item_release�Ŕj�����Ă�������.
//!
//!	@param	name	[in]�\�����O
//!	@param	file	[in]�\���A�C�R���t�@�C��
//!	@param	userdata[in]�Ăяo�����ŗ��p���郆�[�U�[�f�[�^
//!	@param	item	[out]�쐬���ꂽ���j���[�A�C�e��
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_create_with_icon	( const wchar_t *name, dial_file_t *file, void *userdata, dial_menu_item_t **item );

//!
//!	@brief	���j���[�A�C�e���̔j��
//!
//!	@param	item	[in]�Ώۂ̃A�C�e��
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_release			( dial_menu_item_t *item );

//!
//!	@brief	���[�U�[�f�[�^�̎擾
//!
//!	@param	item	[in]�Ώۂ̃A�C�e��
//!	@param	userdata[out]���[�U�[�f�[�^
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_get_userdata		( dial_menu_item_t *item, void **userdata );

//!
//!	@brief	���j���[���I�����ꂽ�R�[���o�b�N�֐�
//!
//!	@param	item	[in]�I�����ꂽ�A�C�e��
//!
typedef void (TRIGLAV_WINRT_CALL *dial_menu_item_invoked_f)	(dial_menu_item_t *item);

//!
//!	@brief	���j���[���I�����ꂽ�R�[���o�b�N�̒ǉ�
//!
//!	@param	item	[in]�Ώۂ̃A�C�e��
//!	@param	f		[in]�R�[���o�b�N�֐�
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_add_invoked		( dial_menu_item_t *item, dial_menu_item_invoked_f f );

//!
//!	@brief	���j���[���I�����ꂽ�R�[���o�b�N�̍폜
//!
//!	@param	item	[in]�Ώۂ̃A�C�e��
//!	@param	f		[in]�R�[���o�b�N�֐�
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_remove_invoked	( dial_menu_item_t *item, dial_menu_item_invoked_f f );

//!
//!	@brief	�͈͂̎擾
//!
//!	@param	contact	[in]�R���^�N�g
//!	@param	bounds	[out]�͈�
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_contact_get_bounds		( const dial_contact_t *contact, const dial_rect_t **bounds );

//!
//!	@brief	�ʒu�̎擾
//!
//!	@param	contact	[in]�R���^�N�g
//!	@param	point	[out]�ʒu
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_contact_get_position	( const dial_contact_t *contact, const dial_point_t **point );

//!
//!	@brief	x�擾
//!
//!	@param	contact	[in]�R���^�N�g
//!	@param	x		[out]x
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_x		( const dial_rect_t *r, dial_double_t *x );

//!
//!	@brief	y�擾
//!
//!	@param	contact	[in]�R���^�N�g
//!	@param	y		[out]y
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_y		( const dial_rect_t *r, dial_double_t *y );

//!
//!	@brief	���擾
//!
//!	@param	contact	[in]�R���^�N�g
//!	@param	w		[out]��
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_width	( const dial_rect_t *r, dial_double_t *w );

//!
//!	@brief	�����擾
//!
//!	@param	contact	[in]�R���^�N�g
//!	@param	h		[out]����
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_height	( const dial_rect_t *r, dial_double_t *h );

//!
//!	@brief	x�擾
//!
//!	@param	contact	[in]�R���^�N�g
//!	@param	x		[out]x
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_point_get_x		( const dial_point_t *p, dial_double_t *x );

//!
//!	@brief	y�擾
//!
//!	@param	contact	[in]�R���^�N�g
//!	@param	y		[out]y
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_point_get_y		( const dial_point_t *p, dial_double_t *y );

}
