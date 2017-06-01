//!
//!	@brief	サーフェスダイヤルライブラリ.
//!
//!	@note	Windows::UI::Input::RadialControllerに対応する関数を実装しています.
//!	@note	詳細な振る舞いはmsdnを参照してください.
//!	@note	https://docs.microsoft.com/en-us/uwp/api/windows.ui.input.radialcontroller
//!
//!	@note	すべての関数はdial_resultを返します.
//!	@note	関数の結果はout引数で受け取ります. 失敗した場合out引数に変更を加えません.
//!
#pragma once

#include <stdint.h>

#ifndef TRIGLAV_WINRT_API
#define TRIGLAV_WINRT_API __declspec(dllimport)
#endif
#define TRIGLAV_WINRT_CALL __cdecl

extern "C" {

//!	@brief	関数の呼び出し結果.
typedef int32_t	dial_result_t;
static const dial_result_t	dial_ok = 0;		//!< 成功.
static const dial_result_t	dial_failed = 1;	//!< 失敗.

//!	@brief	int
typedef int32_t	dial_int_t;

//!	@breif	bool
typedef int32_t	dial_bool_t;
static const dial_bool_t	dial_false = 0;		//!< 成功.
static const dial_bool_t	dial_true = 1;		//!< 失敗.

//!	@brief	double
typedef double	dial_double_t;

//!	@brief	汎用ハンドル型
typedef void *	dial_handle_t;

//!	@breif	ダイヤルコントローラ.
typedef struct _dial_controller		dial_controller_t;

//!	@brief	ダイヤルメニューアイテム.
typedef struct _dial_menu_item		dial_menu_item_t;

//!	@brief	ダイヤルファイル.
typedef struct _dial_file			dial_file_t;

//!	@biref	ダイヤルコンタクト.(サーフェスダイヤルの位置と範囲を表す).
typedef struct _dial_contact		dial_contact_t;

//!	@brief	Rect
typedef struct _dial_rect			dial_rect_t;

//!	@brief	Point
typedef struct _dial_point			dial_point_t;

//!
//!	@brief	dialの初期化. ほかのdial関数を呼び出す前に必ず呼び出してください.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_initialize	();

//!
//!	@brief	dialの終了処理. 呼び出し後、ほかのあらゆるdial関数を呼び出してはいけません.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_uninitialize();

//!
//!	@brief	サーフェスダイヤルをサポートしているか.
//!
//!	@param	
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_is_supported			( dial_bool_t *supported );

//!
//!	@brief	サーフェスダイヤルコントローラーの作成. 使い終わったらdial_controller_releaseで破棄してください.
//!
//!	@param	hwnd		[in]対象のウインドウ
//!	@param	userdata	[in]呼び出し側で自由に使用. get_userdataで取得できる.
//!	@param	controller	[out]作成されたコントローラー.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_create		( dial_handle_t hwnd, void *userdata, dial_controller_t **controller );

//!
//!	@brief	サーフェスダイヤルコントローラーの破棄
//!
//!	@param	controller	[in]対象のコントローラー.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_release		( dial_controller_t *controller );

//!
//!	@brief	ユーザーデータの取得.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	userdata	[out]dial_controller_createで指定したユーザーデータ.
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_get_userdata	( dial_controller_t *controller, void **userdata );

//!
//!	@brief	回転角度の取得
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	degree		[out]回転角度.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_get_rotation	( dial_controller_t *controller, dial_double_t *degree );

//!
//!	@brief	回転角度の設定. この角度毎に指定したdial_controller_rotation_changed_fが呼び出される.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	degree		[in]回転角度.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_set_rotation	( dial_controller_t *controller, dial_double_t degree );

//!
//!	@brief	回転のフィードバックは有効か.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	enabled		[out]dial_true有効, dial_false無効
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_is_feedback	( dial_controller_t *controller, dial_bool_t *enabled );

//!
//!	@brief	回転のフィードバックの設定.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	enabled		[in]dial_true有効, dial_false無効
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_set_feedback	( dial_controller_t *controller, dial_bool_t enabled );

//!	@brief	システムメニューアイテム.(非公開)
typedef enum dial_default_menu_item dial_default_menu_item_t;

//!
//!	@brief	システムメニューの設定.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	items		[in]設定するシステムメニューの配列
//!	@param	item_count	[in]itemsの数
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_configuration_set_default_menu_items			( dial_controller_t *controller, dial_default_menu_item_t *items, dial_int_t item_count );

//!
//!	@brief	システムメニューの選択を試みる
//!
//!	@param	controller	[in]対象のコントローラー
//!	@param	item		[in]選択対象
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_configuration_try_select_default_menu_item	( dial_controller_t *controller, dial_default_menu_item_t item );

//!
//!	@brief	メニューをリセットする.
//!
//!	@param	controller	[in]対象のコントローラー
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_configuration_reset_to_default_menu_items	( dial_controller_t *controller );

//!
//!	@brief	クリックのコールバック関数.
//!
//!	@param	controller	[in]呼び出し元のコントローラ
//!	@param	contact		[in]コンタクト
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_clicked_f)			(dial_controller_t *controller, const dial_contact_t *contact);

//!
//!	@brief	コントロールの獲得のコールバック関数.
//!
//!	@param	controller	[in]呼び出し元のコントローラ
//!	@param	contact		[in]コンタクト
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_acquired_f)			(dial_controller_t *controller, const dial_contact_t *contact);

//!
//!	@brief	コントロールの喪失コールバック関数.
//!
//!	@param	controller	[in]呼び出し元のコントローラ
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_control_lost_f)		(dial_controller_t *controller);

//!
//!	@brief	回転のコールバック関数.
//!
//!	@param	controller	[in]呼び出し元のコントローラ
//!	@param	contact		[in]コンタクト
//!	@param	delta		[in]回転量(°)
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_rotation_changed_f)	(dial_controller_t *controller, const dial_contact_t *contact, const dial_double_t delta);

//!
//!	@brief	接触継続のコールバック関数
//!
//!	@param	controller	[in]呼び出し元のコントローラ
//!	@param	contact		[in]コンタクト
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_contact_continue_f)	(dial_controller_t *controller, const dial_contact_t *contact);

//!
//!	@brief	接触終了のコールバック関数
//!
//!	@param	controller	[in]呼び出し元のコントローラ
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_contact_ended_f)		(dial_controller_t *controller);

//!
//!	@brief	接触開始のコールバック関数
//!
//!	@param	controller	[in]呼び出し元のコントローラ
//!	@param	contact		[in]コンタクト
//!
typedef void (TRIGLAV_WINRT_CALL *dial_controller_contact_started_f)	(dial_controller_t *controller, const dial_contact_t *contact);

//!
//!	@brief	クリックコールバック関数の追加.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]コールバック関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_clicked			( dial_controller_t *controller, dial_controller_clicked_f f );

//!
//!	@brief	獲得コールバック関数の追加.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]コールバック関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_acquired			( dial_controller_t *controller, dial_controller_acquired_f f );

//!
//!	@brief	喪失コールバック関数の追加.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]コールバック関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_control_lost		( dial_controller_t *controller, dial_controller_control_lost_f f );

//!
//!	@brief	回転コールバック関数の追加.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]コールバック関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_rotation_changed	( dial_controller_t *controller, dial_controller_rotation_changed_f f );

//!
//!	@brief	接触継続コールバック関数の追加.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]コールバック関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_contact_continue	( dial_controller_t *controller, dial_controller_contact_continue_f f );

//!
//!	@brief	接触終了コールバック関数の追加.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]コールバック関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_contact_ended	( dial_controller_t *controller, dial_controller_contact_ended_f f );

//!
//!	@brief	接触開始コールバック関数の追加.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]コールバック関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_add_contact_started	( dial_controller_t *controller, dial_controller_contact_started_f f );

//!
//!	@brief	クリックコールバック関数の削除.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]対象の関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_clicked			( dial_controller_t *controller, dial_controller_clicked_f f );

//!
//!	@brief	獲得コールバック関数の削除.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]対象の関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_acquired			( dial_controller_t *controller, dial_controller_acquired_f f );

//!
//!	@brief	喪失コールバック関数の削除.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]対象の関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_control_lost		( dial_controller_t *controller, dial_controller_control_lost_f f );

//!
//!	@brief	回転コールバック関数の削除.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]対象の関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_rotation_changed	( dial_controller_t *controller, dial_controller_rotation_changed_f f );

//!
//!	@brief	接触継続コールバック関数の削除.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]対象の関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_contact_continue	( dial_controller_t *controller, dial_controller_contact_continue_f f );

//!
//!	@brief	接触終了コールバック関数の削除.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]対象の関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_contact_ended		( dial_controller_t *controller, dial_controller_contact_ended_f f );

//!
//!	@brief	接触開始コールバック関数の削除.
//!
//!	@param	controller	[in]対象のコントローラー.
//!	@param	f			[in]対象の関数.
//!
//!	@return	dial_reuslt.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_remove_contact_started	( dial_controller_t *controller, dial_controller_contact_started_f f );

//!
//!	@brief	メニューアイテムを設定する
//!
//!	@param	controller	[in]対象のコントローラー
//!	@param	items		[in]アイテムの配列
//!	@param	item_count	[in]itemsの数
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_set_items				( dial_controller_t *controller, dial_menu_item_t **items, dial_int_t item_count );

//!
//!	@brief	選択状態のアイテムを取得
//!
//!	@param	controller	[in]対象のコントローラー
//!	@param	item		[out]選択状態のアイテム. システムメニューや選択されていないときNULL
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_get_selected_menu_item	( dial_controller_t *controller, dial_menu_item_t **item );

//!
//!	@brief	選択状態のアイテムを設定
//!
//!	@param	controller	[in]対象のコントローラー
//!	@param	item		[in]選択するアイテム
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_set_selected_menu_item	( dial_controller_t *controller, dial_menu_item_t *items );

//!
//!	@brief	前回選択されたアイテムの選択を試みる
//!
//!	@param	controller	[in]対象のコントローラー
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_try_select_previous		( dial_controller_t *controller );

//!
//!	@brief	メニューが有効か
//!
//!	@param	controller	[in]対象のコントローラー
//!	@param	enabled		[out]dial_true有効, dial_false無効
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_is_enabled				( dial_controller_t *controller, dial_bool_t *enabled );

//!
//!	@brief	メニューの有効無効を設定
//!
//!	@param	controller	[in]対象のコントローラー
//!	@param	enabled		[in]dial_true有効, dial_false無効
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_controller_menu_set_enabled				( dial_controller_t *controller, dial_bool_t enabled );

//!
//!	@brief	ファイル作成コールバック関数
//!
//!	@param	file	[in]作成されたファイル. 失敗したときNULL. dial_file_releaseで破棄すること.
//!	@param	result	[in]結果
//!
typedef void (TRIGLAV_WINRT_CALL* dial_file_created_f)( dial_file_t *file, dial_result_t result );

//!
//!	@brief	ファイルの作成. 結果はコールバックで受け取ります.
//!
//!	@param	path	[in]ファイルパス
//!	@param	userdata[in]呼び出し側が使用できるユーザーデータ
//!	@param	f		[in]ファイル作成関数
//!
//!	@return	dial_reuslt. dial_okを返すときコールバックは呼び出されます.
//!
//!	@note	コールバックが呼び出されるスレッドはdial_initializeの初期化方法によって異なります.(たぶん)
//!	@note	シングルスレッドで初期化した場合はメインスレッド、マルチスレッドで初期化した場合はワーカースレッドでコールバックが実行されます.
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_file_create			( const wchar_t *path, void *userdata, dial_file_created_f f );

//!
//!	@brief	ファイルの破棄
//!
//!	@param	file	[in]ファイル
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_file_release		( dial_file_t *file );

//!
//!	@brief	ファイルパスの取得
//!
//!	@param	file	[in]ファイル
//!	@param	path	[out]ファイルパス
//!	@param	len		[out]pathの文字数
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_file_get_path		( dial_file_t *file, const wchar_t **path, dial_int_t *len );

//!
//!	@brief	ユーザーデータの取得
//!
//!	@param	file	[in]ファイル
//!	@param	userdata[out]ユーザーデータ
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_file_get_userdata	( dial_file_t *file, void **userdata );

//!
//!	@brief	システムアイコン.
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
//!	@brief	メニューアイテムの作成. dial_menu_item_releaseで破棄してください.
//!
//!	@param	name	[in]表示名前
//!	@param	icon	[in]表示アイコン
//!	@param	userdata[in]呼び出し側で利用するユーザーデータ
//!	@param	item	[out]作成されたメニューアイテム
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_create			( const wchar_t *name, const dial_icon_t icon, void *userdata, dial_menu_item_t **item );

//!
//!	@brief	メニューアイテムの作成. dial_menu_item_releaseで破棄してください.
//!
//!	@param	name	[in]表示名前
//!	@param	file	[in]表示アイコンファイル
//!	@param	userdata[in]呼び出し側で利用するユーザーデータ
//!	@param	item	[out]作成されたメニューアイテム
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_create_with_icon	( const wchar_t *name, dial_file_t *file, void *userdata, dial_menu_item_t **item );

//!
//!	@brief	メニューアイテムの破棄
//!
//!	@param	item	[in]対象のアイテム
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_release			( dial_menu_item_t *item );

//!
//!	@brief	ユーザーデータの取得
//!
//!	@param	item	[in]対象のアイテム
//!	@param	userdata[out]ユーザーデータ
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_get_userdata		( dial_menu_item_t *item, void **userdata );

//!
//!	@brief	メニューが選択されたコールバック関数
//!
//!	@param	item	[in]選択されたアイテム
//!
typedef void (TRIGLAV_WINRT_CALL *dial_menu_item_invoked_f)	(dial_menu_item_t *item);

//!
//!	@brief	メニューが選択されたコールバックの追加
//!
//!	@param	item	[in]対象のアイテム
//!	@param	f		[in]コールバック関数
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_add_invoked		( dial_menu_item_t *item, dial_menu_item_invoked_f f );

//!
//!	@brief	メニューが選択されたコールバックの削除
//!
//!	@param	item	[in]対象のアイテム
//!	@param	f		[in]コールバック関数
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_menu_item_remove_invoked	( dial_menu_item_t *item, dial_menu_item_invoked_f f );

//!
//!	@brief	範囲の取得
//!
//!	@param	contact	[in]コンタクト
//!	@param	bounds	[out]範囲
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_contact_get_bounds		( const dial_contact_t *contact, const dial_rect_t **bounds );

//!
//!	@brief	位置の取得
//!
//!	@param	contact	[in]コンタクト
//!	@param	point	[out]位置
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_contact_get_position	( const dial_contact_t *contact, const dial_point_t **point );

//!
//!	@brief	x取得
//!
//!	@param	contact	[in]コンタクト
//!	@param	x		[out]x
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_x		( const dial_rect_t *r, dial_double_t *x );

//!
//!	@brief	y取得
//!
//!	@param	contact	[in]コンタクト
//!	@param	y		[out]y
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_y		( const dial_rect_t *r, dial_double_t *y );

//!
//!	@brief	幅取得
//!
//!	@param	contact	[in]コンタクト
//!	@param	w		[out]幅
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_width	( const dial_rect_t *r, dial_double_t *w );

//!
//!	@brief	高さ取得
//!
//!	@param	contact	[in]コンタクト
//!	@param	h		[out]高さ
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_rect_get_height	( const dial_rect_t *r, dial_double_t *h );

//!
//!	@brief	x取得
//!
//!	@param	contact	[in]コンタクト
//!	@param	x		[out]x
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_point_get_x		( const dial_point_t *p, dial_double_t *x );

//!
//!	@brief	y取得
//!
//!	@param	contact	[in]コンタクト
//!	@param	y		[out]y
//!
//!	@return	dial_reuslt
//!
TRIGLAV_WINRT_API dial_result_t TRIGLAV_WINRT_CALL dial_point_get_y		( const dial_point_t *p, dial_double_t *y );

}
