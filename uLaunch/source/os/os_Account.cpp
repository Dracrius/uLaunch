#include <os/os_Account.hpp>
#include <util/util_Convert.hpp>
#include <db/db_Save.hpp>
#include <fs/fs_Stdio.hpp>

namespace os {

    std::string GetIconCacheImagePath(const AccountUid user_id) {
        const auto uid_str = util::Format128NintendoStyle(user_id);
        return UL_BASE_SD_DIR "/user/" + uid_str + ".jpg";
    }

    Result QuerySystemAccounts(const bool dump_icon, std::vector<AccountUid> &out_accounts) {
        AccountUid uids[ACC_USER_LIST_SIZE] = {};
        s32 acc_count = 0;
        UL_RC_TRY(accountListAllUsers(uids, ACC_USER_LIST_SIZE, &acc_count));
        for(s32 i = 0; i < acc_count; i++) {
            const auto uid = uids[i];
            out_accounts.push_back(uid);
            if(dump_icon) {
                AccountProfile prof;
                if(R_SUCCEEDED(accountGetProfile(&prof, uid))) {
                    u32 img_size = 0;
                    accountProfileGetImageSize(&prof, &img_size);
                    if(img_size > 0) {
                        auto img_buf = new u8[img_size]();
                        u32 tmp_size;
                        if(R_SUCCEEDED(accountProfileLoadImage(&prof, img_buf, img_size, &tmp_size))) {
                            const auto cache_icon_path = GetIconCacheImagePath(uid);
                            fs::WriteFile(cache_icon_path, img_buf, img_size, true);
                        }
                        delete[] img_buf;
                    }
                    accountProfileClose(&prof);
                }
            }
        }
        return ResultSuccess;
    }

    Result GetAccountName(const AccountUid user_id, std::string &out_name) {
        AccountProfile prof;
        UL_RC_TRY(accountGetProfile(&prof, user_id));
        UL_ON_SCOPE_EXIT({ accountProfileClose(&prof); });
    
        AccountProfileBase pbase;
        AccountUserData udata;
        UL_RC_TRY(accountProfileGet(&prof, &udata, &pbase));

        out_name = pbase.nickname;
        return ResultSuccess;
    }
}