// privilege.hpp
//
// Type Definitions
//  - wdl::security::account_privilege_action
//  - wdl::security::privilege_action
//
// Free Function Definitions:
//	- wdl::security::open_lsa_policy()
//  - wdl::security::set_account_privilege()
//  - wdl::security::set_privilege()
//
// Utility functions for working with Windows access tokens.

#pragma once

#include <windows.h>
#include <ntsecapi.h>
#include <sddl.h>

#include <memory>

#include <wdl/security/base.hpp>
#include <wdl/error/nt_exception.hpp>
#include <wdl/error/win32_exception.hpp>

namespace wdl::security
{
    namespace detail
    {
        void init_lsa_string(PLSA_UNICODE_STRING lsa_string, LPWSTR string)
        {
            if (nullptr == string) 
            {
                lsa_string->Buffer = NULL;
                lsa_string->Length = 0;
                lsa_string->MaximumLength = 0;
                return;
            }

            auto const length = ::wcslen(string);

            lsa_string->Buffer = string;
            lsa_string->Length 
                = static_cast<unsigned short>(length * sizeof(wchar_t));
            lsa_string->MaximumLength 
                = static_cast<unsigned short>((length + 1) * sizeof(wchar_t));
        }
    }

    // wdl::security::account_privilege_action

    enum class account_privilege_action
    {
        add,
        remove
    };

    // wdl::security::privilege_action

    enum class privilege_action
    {
        enable,
        disable
    };

    // wdl::security::open_lsa_policy()

    lsa_policy_handle open_lsa_policy(
        wchar_t*            server_name, 
        unsigned long const desired_access
        )
    {
        auto policy_handle = lsa_policy_handle{};

        auto object_attrs = LSA_OBJECT_ATTRIBUTES{};
        auto server_string = LSA_UNICODE_STRING{};
        auto server = PLSA_UNICODE_STRING{};

        ZeroMemory(&object_attrs, sizeof(object_attrs));

        if (server_name != NULL) 
        {
            detail::init_lsa_string(&server_string, server_name);
            server = &server_string;
        }

        auto const status = ::LsaOpenPolicy(
            server,
            &object_attrs,
            desired_access,
            policy_handle.put());
        
        return policy_handle;
    }

    // wdl::security::set_account_privilege()

    void set_account_privilege(
        LSA_HANDLE               policy, 
        PSID                     account_sid, 
        wchar_t*                 privilege_name, 
        account_privilege_action action
        )
    {
        auto privilege_string = LSA_UNICODE_STRING{};
        detail::init_lsa_string(&privilege_string, privilege_name);

        auto status = NTSTATUS{};
        if (action == account_privilege_action::add) 
        {
            status = ::LsaAddAccountRights(
                policy, 
                account_sid,      
                &privilege_string,  
                1);
        }
        else 
        {
            status = ::LsaRemoveAccountRights(
                policy, 
                account_sid, 
                FALSE,  
                &privilege_string,  
                1);
        }

        wdl::error::check_nt(status);
    }

    // wdl::security::set_privilege()

    void set_privilege(
        HANDLE           token, 
        wchar_t const*   name, 
        privilege_action action
        )
    {
        auto luid       = LUID{};
        auto privileges = TOKEN_PRIVILEGES{};

        if (!::LookupPrivilegeValueW(nullptr, name, &luid))
        {
            throw wdl::error::win32_exception{};
        }

        privileges.PrivilegeCount = 1;
        privileges.Privileges[0].Luid = luid;

        if (action == privilege_action::enable)
        {
            privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        }
        else
        {
            privileges.Privileges[0].Attributes = 0;
        }

        if (!::AdjustTokenPrivileges(
            token, 
            FALSE, 
            &privileges, 
            sizeof(TOKEN_PRIVILEGES), 
            nullptr, nullptr))
        {
            throw wdl::error::win32_exception{};
        }

        wdl::error::check_win32(::GetLastError());
    }
}