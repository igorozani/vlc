/*****************************************************************************
 * audio_output.h : audio output interface
 *****************************************************************************
 * Copyright (C) 2002 VideoLAN
 * $Id: audio_output.h,v 1.57 2002/08/11 23:26:28 massiot Exp $
 *
 * Authors: Christophe Massiot <massiot@via.ecp.fr>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111, USA.
 *****************************************************************************/

/*****************************************************************************
 * audio_sample_format_t
 *****************************************************************************
 * This structure defines a format for audio samples.
 *****************************************************************************/
struct audio_sample_format_t
{
    int                 i_format;
    int                 i_rate;
    int                 i_channels;
};

#define AOUT_FMT_MU_LAW     0x00000001
#define AOUT_FMT_A_LAW      0x00000002
#define AOUT_FMT_IMA_ADPCM  0x00000004
#define AOUT_FMT_U8         0x00000008
#define AOUT_FMT_S16_LE     0x00000010            /* Little endian signed 16 */
#define AOUT_FMT_S16_BE     0x00000020               /* Big endian signed 16 */
#define AOUT_FMT_S8         0x00000040
#define AOUT_FMT_U16_LE     0x00000080                  /* Little endian U16 */
#define AOUT_FMT_U16_BE     0x00000100                     /* Big endian U16 */
#define AOUT_FMT_SPDIF      0x00000400            /* S/PDIF hardware support */
#define AOUT_FMT_FLOAT32    0x00010000
#define AOUT_FMT_FIXED32    0x00020000
#define AOUT_FMT_A52        0x00100000
#define AOUT_FMT_DTS        0x00200000

#define AOUT_FMTS_IDENTICAL( p_first, p_second ) (                          \
    ((p_first)->i_format == (p_second)->i_format)                           \
      && ((p_first)->i_rate == (p_second)->i_rate)                          \
      && ((p_first)->i_channels == (p_second)->i_channels                   \
           || (p_first)->i_channels == -1 || (p_second)->i_channels == -1) )

#ifdef WORDS_BIGENDIAN
#   define AOUT_FMT_S16_NE AOUT_FMT_S16_BE
#   define AOUT_FMT_U16_NE AOUT_FMT_U16_BE
#else
#   define AOUT_FMT_S16_NE AOUT_FMT_S16_LE
#   define AOUT_FMT_U16_NE AOUT_FMT_U16_LE
#endif

#define AOUT_FMT_IS_SPDIF( p_format )                                      \
    ( ((p_format)->i_format == AOUT_FMT_SPDIF)                             \
       || ((p_format)->i_format == AOUT_FMT_A52)                           \
       || ((p_format)->i_format == AOUT_FMT_DTS) )

/* This is heavily borrowed from libmad, by Robert Leslie <rob@mars.org> */
/*
 * Fixed-point format: 0xABBBBBBB
 * A == whole part      (sign + 3 bits)
 * B == fractional part (28 bits) 
 *
 * Values are signed two's complement, so the effective range is:
 * 0x80000000 to 0x7fffffff
 *       -8.0 to +7.9999999962747097015380859375
 *
 * The smallest representable value is:
 * 0x00000001 == 0.0000000037252902984619140625 (i.e. about 3.725e-9)
 *
 * 28 bits of fractional accuracy represent about
 * 8.6 digits of decimal accuracy.
 * 
 * Fixed-point numbers can be added or subtracted as normal
 * integers, but multiplication requires shifting the 64-bit result
 * from 56 fractional bits back to 28 (and rounding.)
 */
typedef s32 vlc_fixed_t;
#define FIXED32_FRACBITS 28
#define FIXED32_MIN ((vlc_fixed_t) -0x80000000L)
#define FIXED32_MAX ((vlc_fixed_t) +0x7fffffffL)
#define FIXED32_ONE ((vlc_fixed_t) 0x10000000)



/*****************************************************************************
 * aout_buffer_t : audio output buffer
 *****************************************************************************/
struct aout_buffer_t
{
    byte_t *                p_buffer;
    int                     i_alloc_type;
    size_t                  i_size;
    int                     i_nb_samples;
    mtime_t                 start_date, end_date;

    struct aout_buffer_t *  p_next;
};

/* Size of a frame for S/PDIF output. */
#define AOUT_SPDIF_SIZE 6144

/*****************************************************************************
 * Prototypes
 *****************************************************************************/
/* From audio_output.c : */
#define aout_NewInstance(a) __aout_NewInstance(VLC_OBJECT(a))
VLC_EXPORT( aout_instance_t *, __aout_NewInstance,    ( vlc_object_t * ) );
VLC_EXPORT( void,              aout_DeleteInstance, ( aout_instance_t * ) );
VLC_EXPORT( aout_buffer_t *, aout_BufferNew, ( aout_instance_t *, aout_input_t *, size_t ) );
VLC_EXPORT( void, aout_BufferDelete, ( aout_instance_t *, aout_input_t *, aout_buffer_t * ) );
VLC_EXPORT( void, aout_BufferPlay, ( aout_instance_t *, aout_input_t *, aout_buffer_t * ) );
VLC_EXPORT( int, aout_FormatTo, ( audio_sample_format_t * p_format, int ) );
#define aout_FormatToByterate(a,b) aout_FormatTo(a,b)
#define aout_FormatToSize(a,b) aout_FormatTo(a,b)

/* From input.c : */
#define aout_InputNew(a,b,c) __aout_InputNew(VLC_OBJECT(a),b,c)
VLC_EXPORT( aout_input_t *, __aout_InputNew, ( vlc_object_t *, aout_instance_t **, audio_sample_format_t * ) );
VLC_EXPORT( void, aout_InputDelete, ( aout_instance_t *, aout_input_t * ) );

/* From output.c : */
VLC_EXPORT( aout_buffer_t *, aout_OutputNextBuffer, ( aout_instance_t *, mtime_t ) );

