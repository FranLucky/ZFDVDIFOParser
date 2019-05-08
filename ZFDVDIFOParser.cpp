//
//  ZFDVDIFOParser.cpp
//  ZFDVDIFO
//
//  Created by 张帆 on 2019/5/7.
//  Copyright © 2019 张帆. All rights reserved.
//

#include "ZFDVDIFOParser.hpp"


#ifndef AV_RB64
#   define AV_RB64(x)                                   \
(((uint64_t)((const uint8_t*)(x))[0] << 56) |       \
((uint64_t)((const uint8_t*)(x))[1] << 48) |       \
((uint64_t)((const uint8_t*)(x))[2] << 40) |       \
((uint64_t)((const uint8_t*)(x))[3] << 32) |       \
((uint64_t)((const uint8_t*)(x))[4] << 24) |       \
((uint64_t)((const uint8_t*)(x))[5] << 16) |       \
((uint64_t)((const uint8_t*)(x))[6] <<  8) |       \
(uint64_t)((const uint8_t*)(x))[7])
#endif
#ifndef AV_WB64
#   define AV_WB64(p, val) do {                 \
uint64_t d = (val);                     \
((uint8_t*)(p))[7] = (d);               \
((uint8_t*)(p))[6] = (d)>>8;            \
((uint8_t*)(p))[5] = (d)>>16;           \
((uint8_t*)(p))[4] = (d)>>24;           \
((uint8_t*)(p))[3] = (d)>>32;           \
((uint8_t*)(p))[2] = (d)>>40;           \
((uint8_t*)(p))[1] = (d)>>48;           \
((uint8_t*)(p))[0] = (d)>>56;           \
} while(0)
#endif

#ifndef AV_RB32
#   define AV_RB32(x)                                \
(((uint32_t)((const uint8_t*)(x))[0] << 24) |    \
(((const uint8_t*)(x))[1] << 16) |    \
(((const uint8_t*)(x))[2] <<  8) |    \
((const uint8_t*)(x))[3])
#endif
#ifndef AV_WB32
#   define AV_WB32(p, val) do {                 \
uint32_t d = (val);                     \
((uint8_t*)(p))[3] = (d);               \
((uint8_t*)(p))[2] = (d)>>8;            \
((uint8_t*)(p))[1] = (d)>>16;           \
((uint8_t*)(p))[0] = (d)>>24;           \
} while(0)
#endif

#ifndef AV_RB16
#   define AV_RB16(x)                                \
(((uint16_t)((const uint8_t*)(x))[0] <<  8) |    \
((const uint8_t*)(x))[1])
#endif


#define AV_RB8(x)     (((const uint8_t*)(x))[0])
#define AV_WB8(p, d)  do { ((uint8_t*)(p))[0] = (d); } while(0)


struct ifo_t {
    
};

unsigned char *avjump(unsigned char *p, int l) {
    p+=l;
    return p;
}


int ParserIfo(unsigned char *p, long size) {
    
    if (size == 0) {
        perror("size == 0");
        return -1;
    }
    
    if (p == NULL) {
        perror("p == NULL");
        return -1;
    }
    
    unsigned char *buffer = p;
    
    if (strncmp((char *)buffer, "DVDVIDEO-VMG", 12) == 0) {
        printf("1\n");
        { // who konws
            uint32_t last_sector_vmg = AV_RB32(buffer + 0xc);
            printf("last_sector_vmg:%d \n", last_sector_vmg);
            uint32_t last_sector_ifo = AV_RB32(buffer + 0x1c);
            printf("last_sector_ifo:%d \n", last_sector_ifo);
            
        }
        { // version
            unsigned char *pV = buffer + 0x20;
            uint8_t flag = pV[0];
            if (flag != 0) {
                perror("version flag not 0");
                return -1;
            }
            uint8_t version = pV[1];
            uint8_t major = version >>4;
            uint8_t minor = version & 0xf;
            printf("version: v%d.%d \n", major, minor);
        }
        { // VMG category
            uint8_t regin_mask = AV_RB8(buffer + 0x22 + 1); // byte = 1
            printf("regin_mask:%d \n", regin_mask);
        }
        {
            // number of volumes
            uint16_t num_of_vol = AV_RB16(buffer + 0x26);
            printf("number of volumes:%d \n", num_of_vol);
            
            // volume number
            uint16_t vol_num = AV_RB16(buffer + 0x28);
            printf("volume number:%d \n", vol_num);
        }
        {
            uint8_t side_id = AV_RB8(buffer + 0x2a);
            printf("size_id:%d\n", side_id);
        }
        
        { // number of title sets
            uint16_t title_num = AV_RB16(buffer + 0x3e);
            printf("title_num:%d\n", title_num);
        }
        
        {
            // skip Provider ID
            // skip VMG POS
        }
        
        { // end byte address of VMGI_MAT
            uint32_t end_of_mat = AV_RB32(buffer + 0x80);
            printf("end_of_mat:%d\n", end_of_mat);
        }
        { // parser address
            // start address of FP_PGC
            uint32_t start_of_pgc = AV_RB32(buffer + 0x84);
            printf("start_of_pgc:%d\n", start_of_pgc);
            
            // start sector of Menu VOB
            uint32_t start_sector_vob = AV_RB32(buffer + 0xc0);
            printf("start_sector_vob:%d\n", start_sector_vob);
            
            // sector pointer to TT_SRPT (table of titles)
            uint32_t sector_p_srpt = AV_RB32(buffer + 0xc4);
            printf("sector_p_srpt:%d\n", sector_p_srpt);
            
            // sector pointer to VMGM_PGCI_UT (Menu Program Chain table)
            uint32_t sector_p_ut = AV_RB32(buffer + 0xc8);
            printf("sector_p_ut:%d\n", sector_p_ut);
            
            // sector pointer to VMG_PTL_MAIT (Parental Management masks)
            uint32_t sector_p_mait = AV_RB32(buffer + 0xcc);
            printf("sector_p_mait:%d\n", sector_p_mait);
            
            // sector pointer to VMG_VTS_ATRT (copies of VTS audio/sub-picture attributes)
            uint32_t sector_p_atrt = AV_RB32(buffer + 0xd0);
            printf("sector_p_atrt:%d\n", sector_p_atrt);
            
            // sector pointer to VMG_TXTDT_MG (text data)
            uint32_t sector_p_mg = AV_RB32(buffer + 0xd4);
            printf("sector_p_mg:%d\n", sector_p_mg);
            
            // sector pointer to VMGM_C_ADT (menu cell address table)
            uint32_t sector_p_adt = AV_RB32(buffer + 0xd8);
            printf("sector_p_adt:%d\n", sector_p_adt);
            
            // sector pointer to VMGM_VOBU_ADMAP (menu VOBU address map)
            uint32_t sector_p_admap = AV_RB32(buffer + 0xdc);
            printf("sector_p_admap:%d\n", sector_p_admap);
            
        }
        
        { // video attributes of VMGM_VOBS
            unsigned char *video_attr = buffer + 0x100;
            uint8_t video_attr0 = AV_RB8(video_attr);
            
            uint8_t coding_mode = video_attr0 >> 6;
            switch (coding_mode) {
                case 0:
                    printf("Coding-Mode: Mpeg-1\n");
                    break;
                case 1:
                    printf("Coding-Mode: Mpeg-2\n");
                    break;
                default:
                    printf("undefind coding_mode :%d \n", coding_mode);
                    break;
            }
            
            uint8_t standard = (video_attr0 & 0x30) >> 4;
            switch (standard) {
                case 0:
                    printf("standard: NTSC\n");
                    break;
                case 1:
                    printf("standard: PAL\n");
                    break;
                default:
                    printf("undefind standard :%d \n", standard);
                    break;
            }
            
            uint8_t aspect = (video_attr0 & 0xc) >> 2;
            switch (aspect) {
                case 0:
                    printf("aspect: 4:3\n");
                    break;
                case 1:
                case 2:
                    printf("aspect: reserved\n");
                    break;
                case 3:
                    printf("aspect: 16:9\n");
                    break;
                default:
                    printf("undefind aspect :%d \n", aspect);
                    break;
            }
            
            uint8_t automatic_Pan_or_Scan = (video_attr0 & 0x2) >> 1;
            printf("Automatic Pan/Scan : %s\n", (automatic_Pan_or_Scan==1) ? "disallowed" : "allowed");
            
            uint8_t automatic_Letterbox = video_attr0 & 0x1;
            printf("Automatic Letterbo : %s\n", (automatic_Letterbox==1) ? "disallowed" : "allowed");

            
           
            
            uint8_t video_attr1 = AV_RB8(video_attr+1);
            /*
             * Skip (dont know whats this)
             * CC for line 21 field 1 in GOP (NTSC only) bit = 8
             * CC for line 21 field 2 in GOP (NTSC only) bit = 7
             */
            uint8_t resolution  = (video_attr1 & 0x38) >> 3;
            switch (resolution) {
                case 0:
                    printf("resolution: 720x480 (720x576)\n");
                    break;
                case 1:
                    printf("resolution: 704x480 (704x576)\n");
                    break;
                case 2:
                    printf("resolution: 352x480 (352x576)\n");
                    break;
                case 3:
                    printf("resolution: 352x240 (352x288)\n");
                    break;
                default:
                    printf("undefind resolution :%d \n", resolution);
                    break;
            }
            
            uint8_t letterboxed  = (video_attr1 & 0x4) >> 2;
            printf("letterboxed : %s\n", (letterboxed==1) ? "yes" : "no");
            
            if (standard == 1) { // pal
                uint8_t mode  = (video_attr1 & 0x1);
                printf("mode : %s\n", (mode==1) ? "film" : "camera");
            }
            
        }
        
        { // number of audio streams in VMGM_VOBS
            uint16_t num_of_a_streams_vobs = AV_RB16(buffer + 0x102);
            printf("num_of_a_streams_vobs : %d\n", num_of_a_streams_vobs);
        }
        
        { // audio attr
            
            unsigned char *audio_attr = buffer + 0x104;
            
            uint8_t audio_attr0 = AV_RB8(audio_attr);
            uint8_t coding_mode = audio_attr0 >> 5;
            switch (coding_mode) {
                case 0:
                    printf("Audio-Coding-Mode: AC3\n");
                    break;
                case 3:
                    printf("Audio-Coding-Mode: Mpeg-2ext\n");
                    break;
                case 4:
                    printf("Audio-Coding-Mode: LPCM \n");
                    break;
                case 6:
                    printf("Audio-Coding-Mode: DTS \n");
                    break;
                default:
                    printf("undefind Audio-coding_mode :%d \n", coding_mode);
                    break;
            }
//            uint8_t present_multi_extension = audio_attr0 & 0x10 >> 4;
            
            uint8_t language_type = audio_attr0 & 0xc >> 2;
            bool need_parser_bytes_2_3 = false;
            switch (language_type) {
                case 0:
                    printf("Audio-language_type: unspecified\n");
                    break;
                case 1:
                    need_parser_bytes_2_3 = true;
                    break;
                default:
                    printf("undefind Audio-coding_mode :%d \n", language_type);
                    break;
            }
            
            uint8_t application_mode = audio_attr0 & 0x3;
            switch (application_mode) {
                case 0:
                    printf("Audio-application_mode: unspecified\n");
                    break;
                case 1:
                    printf("Audio-application_mode: karaoke\n");
                    break;
                case 2:
                    printf("Audio-application_mode: surround\n");
                    break;
                default:
                    printf("undefind Audio-application_mode :%d \n", application_mode);
                    break;
            }
            
            
            
            uint8_t audio_attr1 = AV_RB8(audio_attr+1);
            uint8_t quantization = audio_attr1 >> 6;
            if (coding_mode == 2 || coding_mode == 3) {
                switch (quantization) {
                    case 0:
                        printf("quantization: No DRC\n");
                        break;
                    case 1:
                        printf("quantization: DRC\n");
                        break;
                    default:
                        printf("undefind quantization :%d \n", quantization);
                        break;
                }
            } else if (coding_mode == 4) {
                switch (quantization) {
                    case 0:
                        printf("quantization: 16bps\n");
                        break;
                    case 1:
                        printf("quantization: 20bps\n");
                        break;
                    case 2:
                        printf("quantization: 24bps\n");
                        break;
                    default:
                        printf("undefind quantization :%d \n", quantization);
                        break;
                }
            }
            
            uint8_t sample_rate = audio_attr1 & 0x10 >> 4;
            printf("Sample rate: %dKbps", sample_rate);
            // skip bit 1 bit 0 it means:channels-1
            
            
            if (need_parser_bytes_2_3) {
                char audio_attr2 = AV_RB8(audio_attr+2);
                char audio_attr3 = AV_RB8(audio_attr+3);
                char audio_attr4 = AV_RB8(audio_attr+4);
                printf("Language type: %c%c%c", audio_attr2, audio_attr3, audio_attr4);
            }
            
            
            uint8_t audio_attr5 = AV_RB8(audio_attr+5);
            switch (audio_attr5) {
                case 0:
                    printf("code extension: unspecified\n");
                    break;
                case 1:
                    printf("code extension: normal\n");
                    break;
                case 2:
                    printf("code extension: for visually impaired\n");
                    break;
                case 3:
                    printf("code extension: director's comments\n");
                    break;
                case 4:
                    printf("code extension: alternate director's comments\n");
                    break;
                    
                default:
                    printf("undefind code extension :%d \n", audio_attr5);
                    break;
            }
            if (application_mode == 1) {
                uint8_t audio_attr6 = AV_RB8(audio_attr+6);
                // TODO: parser  Application information karaoke mode
            }
            if (application_mode == 2) {
                uint8_t audio_attr7 = AV_RB8(audio_attr+7);
                // TODO: parser  Application information surround mode
            }
            
            
        }
        
    } else if (strncmp((char *)buffer, "DVDVIDEO-VTS", 12) == 0) {
        printf("2");
    }
    
    
    
    return 0;
}

