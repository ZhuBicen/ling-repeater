#ifndef LING_UI_HPP_INCLUDED
#define LING_UI_HPP_INCLUDED
/////////////////////////////
//implement UiInterface
////////////////////////////
#include "UiInterface.hpp"
#define WM_STARTTIMER                  WM_USER + 0x001
#define WM_STOPTIMER                   WM_USER + 0x002
#define WM_SETFILEINFO                 WM_USER + 0x003
#define WM_EXITAPP                     WM_USER + 0x004
#define WM_SETCONFINFO                     WM_USER + 0x005
#define WM_DRAWBAR                     WM_USER + 0x006
#define WM_CONTEXTMENUINFO             WM_USER + 0x007



template< class WinImpl>
class Ui:public UiInterface
{    
public:
    Ui(){
        impl_ = static_cast<WinImpl*>(this);
    }
    
    void StartTimer(){
        PostMessage_(WM_STARTTIMER, 0, 0);
    }
    void StopTimer(){
        PostMessage_(WM_STOPTIMER, 0, 0);
    }


    void SetFileInfo(const FileInfo& file_info, long file_length){
        FileInfo* p_file_info = new FileInfo();
        *p_file_info = file_info;
        PostMessage_(WM_SETFILEINFO, (WPARAM)p_file_info, file_length);
    }

    //void StartPlay(long pos){
    //    PaintInfo* pi = new PaintInfo();
    //    pi->current_pos_ = pos;
    //    PostMessage_(WM_DRAWBAR, (WPARAM) pi, 0);
    //}

    void SetStartPos(long pos){
        //impl_->PostMessage(WM_SETSTARTPOS, pos, 0);
    }

    void SetEndPos(long pos){
        //impl_->PostMessage(WM_SETENDPOS, pos, 0);
    }

    //void StartRepeat(long start_pos, long end_pos){
    //    DrawRepeatedProgressBar(start_pos, end_pos, start_pos);
    //}

    void ExitApp(){
        PostMessage_(WM_EXITAPP, 0, 0);
    }

    void SetConfInfo(const char* p_media_repo){
        PostMessage_(WM_SETREPO, (int)p_media_repo, 0);
    }

    void DrawBar(PaintInfo* pi){
        PostMessage_(WM_DRAWBAR, (WPARAM) pi, 0);
    }
    void UpdateBar(PaintUpdateInfo* pui){
        PostMessage_(WM_DRAWBAR, (WPARAM) pui, 1);
    }

    void ShowContextMenu(bool mark, int x , int y){
        ContextMenuInfo* pcmi = new ContextMenuInfo();
        pcmi->mark_ = mark;
        pcmi->x_ = x;
        pcmi->y_  = y;
        if(mark){
            PostMessage_(WM_CONTEXTMENUINFO, (WPARAM)pcmi, 0);
        }else{
            PostMessage_(WM_CONTEXTMENUINFO, (WPARAM)pcmi, 0);
        }
    }
private:
    void PostMessage_(UINT msg_id, WPARAM w, LPARAM l){
        // impl_->PostMessage(msg_id, w, l);
        if(::PostMessage(impl_->m_hWnd, msg_id, w, l)){
            LOG(logDEBUG1) << __FUNCTION__ << std::hex << " (hwnd = 0x" << impl_->m_hWnd << ", msg_id = 0x" << msg_id <<", w = 0x" << w << ", l = 0x" << l << ")" << std::dec;
        }else{
            LOG(logERROR) << __FUNCTION__ << "Failed to post message: " << std::hex << msg_id << std::dec;
        }
    }
    WinImpl* impl_;
};
#endif
