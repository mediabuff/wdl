// message_queue.h
// Class Implementation: wdl::misc::message_queue
//
// Simple message queue implemented via Window events.

#pragma once

#include <windows.h>

namespace wdl
{
    struct message
    {
        UINT_PTR type;
        void*    payload;
    };

    class message_queue
    {
        HWND          window_handle_;
        unsigned long window_owner_thread_id_;

    public:
        message_queue() 
            : window_handle_{ NULL }
            , window_owner_thread_id_{ ::GetCurrentThreadId() }
        {
            WNDCLASS wc      = { 0 };
            wc.lpfnWndProc   = window_procedure;
            wc.hInstance     = GetModuleHandle(NULL);
            wc.lpszClassName = L"DUMMY_NAME";

            if (!RegisterClass(&wc)) return;

            window_handle_ = CreateWindowEx(
                0,
                L"DUMMY_NAME",
                L"DUMMY_NAME",
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                HWND_MESSAGE,
                NULL,
                GetModuleHandle(NULL),
                NULL
                );
        }

        ~message_queue()
        {
            DestroyWindow(window_handle_);
        }

        bool push_back(message& msg)
        {
            return PostMessage(
                window_handle_, 
                WM_USER, 
                msg.type, 
                reinterpret_cast<LONG_PTR>(msg.payload)
                );
        }

        bool pop_front(message& msg)
        {
            BOOL ret;
            auto message = MSG{};
            
            ret = GetMessage(&message, window_handle_, 0, 0);
            if (-1 == ret)
            {
                // error
                return false;
            }

            msg.type    = message.wParam;
            msg.payload = reinterpret_cast<void*>(message.lParam);

            return true;
        }

        explicit operator bool() const noexcept
        {
            return window_handle_ != NULL;
        }

    private:

        // dummy window procedure
        static LRESULT CALLBACK 
        window_procedure(
            HWND   window, 
            UINT   message, 
            WPARAM w_param, 
            LPARAM l_param
            )
        {
            return DefWindowProc(window, message, w_param, l_param);
        }
    };
}