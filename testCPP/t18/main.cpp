//
// Created by Elevoc on 2020/11/20 0020.
//
#include "ElevocLicense.h"
#include <cstring>
#include <cstdio>

#define	fix_a_id	0x4f7a2134L
#define	fix_s_id	0x24292339L
#define	fix_p_id	0x0L
#define	fix_o_id	0xd6892526L
#define	e_key	0xf0
#define	o_tag	"elevocInctestapp1605844004"

#define  appid "1001070"
#define  secreatKey "5tuxaWOAuAgNkDiD"
int main(){
    unsigned int o_id= elevoc_generate_license(0, o_tag, e_key, strlen(o_tag));
    unsigned int a_id = elevoc_generate_license(0, appid, e_key, (int) strlen(appid));
    unsigned int s_id = elevoc_generate_license(0, secreatKey, e_key, (int) strlen(secreatKey));
    printf("o_id 0x%x\n",o_id);
    printf("a_id 0x%x\n",a_id);
    printf("s_id 0x%x\n",s_id);
    return 0;
}