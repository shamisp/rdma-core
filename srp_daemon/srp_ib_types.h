/*
 * srp-ib_types - discover SRP targets over IB
 * Copyright (c) 2005 Topspin Communications.  All rights reserved.
 * Copyright (c) 2006 Cisco Systems, Inc.  All rights reserved.
 * Copyright (c) 2006 Mellanox Technologies Ltd.  All rights reserved.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SRP_IB_TYPES_H
#define SRP_IB_TYPES_H

#include <endian.h>
#include <stdint.h>
#include <linux/types.h> /* __be16, __be32 and __be64 */
#include <infiniband/umad.h> /* union umad_gid */

#define SRP_INFORMINFO_LID_COMP		(1 << 1)
#define SRP_INFORMINFO_ISGENERIC_COMP	(1 << 4)
#define SRP_INFORMINFO_SUBSCRIBE_COMP	(1 << 5)
#define SRP_INFORMINFO_TRAPTYPE_COMP	(1 << 6)
#define SRP_INFORMINFO_TRAPNUM_COMP	(1 << 7)
#define SRP_INFORMINFO_QPN_COMP		(1 << 8)
#define SRP_INFORMINFO_PRODUCER_COMP	(1 << 12)

#ifndef PACK_SUFFIX
#define PACK_SUFFIX __attribute__((packed))
#endif

/****d* IBA Base: Constants/MAD_BLOCK_SIZE
* NAME
*	MAD_BLOCK_SIZE
*
* DESCRIPTION
*	Size of a non-RMPP MAD datagram.
*
* SOURCE
*/
#define MAD_BLOCK_SIZE						256

/****s* IBA Base: Types/ib_sa_mad_t
* NAME
*	ib_sa_mad_t
*
* DESCRIPTION
*	IBA defined SA MAD format. (15.2.1)
*
* SYNOPSIS
*/
#define IB_SA_DATA_SIZE 200

typedef struct _ib_sa_mad
{
	uint8_t		base_ver;
	uint8_t		mgmt_class;
	uint8_t		class_ver;
	uint8_t		method;
	__be16		status;
	__be16		resv;
	__be64		trans_id;
	__be16		attr_id;
	__be16		resv1;
	__be32		attr_mod;

	uint8_t		rmpp_version;
	uint8_t		rmpp_type;
	uint8_t		rmpp_flags;
	uint8_t		rmpp_status;

	__be32		seg_num;
	__be32		paylen_newwin;

	__be64		sm_key;

	__be16		attr_offset;
	__be16		resv3;

	__be64		comp_mask;

	uint8_t		data[IB_SA_DATA_SIZE];
} PACK_SUFFIX ib_sa_mad_t;

static inline uint32_t ib_get_attr_size(const __be16 attr_offset)
{
	return( ((uint32_t)be16toh( attr_offset )) << 3 );
}

/************************************************************
* NAME
*	MAD_RMPP_HDR_SIZE
*
* DESCRIPTION
*	Size of an RMPP header, including the common MAD header.
*
* SOURCE
*/
enum {
  MAD_RMPP_HDR_SIZE = 36,
};

/****s* IBA Base: Types/ib_path_rec_t
* NAME
*	ib_path_rec_t
*
* DESCRIPTION
*	Path records encapsulate the properties of a given
*	route between two end-points on a subnet.
*
* SYNOPSIS
*/
typedef struct _ib_path_rec
{
	uint8_t		resv0[8];
	union umad_gid	dgid;
	union umad_gid	sgid;
	__be16		dlid;
	__be16		slid;
	__be32		hop_flow_raw;
	uint8_t		tclass;
	uint8_t		num_path;
	__be16		pkey;
	__be16		sl;
	uint8_t		mtu;
	uint8_t		rate;
	uint8_t		pkt_life;
	uint8_t		preference;
	uint8_t		resv2[6];

}	PACK_SUFFIX ib_path_rec_t;


/****s* IBA Base: Types/ib_mad_t
* NAME
*	ib_mad_t
*
* DESCRIPTION
*	IBA defined MAD header (13.4.3)
*
* SYNOPSIS
*/
typedef struct _ib_mad
{
	uint8_t		base_ver;
	uint8_t		mgmt_class;
	uint8_t		class_ver;
	uint8_t		method;
	__be16		status;
	__be16		class_spec;
	__be64		trans_id;
	__be16		attr_id;
	__be16		resv;
	__be32		attr_mod;
}	PACK_SUFFIX ib_mad_t;

/****f* IBA Base: Types/ib_mad_init_new
* NAME
*	ib_mad_init_new
*
* DESCRIPTION
*	Initializes a MAD common header.
*
* SYNOPSIS
*/
static inline void
ib_mad_init_new(ib_mad_t* const	p_mad,
		const uint8_t mgmt_class,
		const uint8_t class_ver,
		const uint8_t method,
		const __be64 trans_id,
		const __be16 attr_id,
		const __be32 attr_mod)
{
	p_mad->base_ver = 1;
	p_mad->mgmt_class = mgmt_class;
	p_mad->class_ver = class_ver;
	p_mad->method = method;
	p_mad->status = 0;
	p_mad->class_spec = 0;
	p_mad->trans_id = trans_id;
	p_mad->attr_id = attr_id;
	p_mad->resv = 0;
	p_mad->attr_mod = attr_mod;
}


typedef struct _ib_inform_info
{
	union umad_gid	gid;
	__be16		lid_range_begin;
	__be16		lid_range_end;
	__be16		reserved1;
	uint8_t		is_generic;
	uint8_t		subscribe;
	__be16		trap_type;
	union _inform_g_or_v
	{
		struct _inform_generic
		{
			__be16		trap_num;
			__be32		qpn_resp_time_val;
			uint8_t 	reserved2;
			uint8_t		node_type_msb;
			__be16		node_type_lsb;
		} PACK_SUFFIX generic;

		struct _inform_vend
		{
			__be16		dev_id;
			__be32		qpn_resp_time_val;
			uint8_t		reserved2;
			uint8_t		vendor_id_msb;
			__be16		vendor_id_lsb;
		} PACK_SUFFIX vend;

	}	PACK_SUFFIX g_or_v;

} PACK_SUFFIX ib_inform_info_t;

typedef struct _ib_mad_notice_attr		// Total Size calc  Accumulated
{
	uint8_t		generic_type;		// 1		1

	union _notice_g_or_v
	{
		struct _notice_generic		// 5		6
		{
			uint8_t		prod_type_msb;
			__be16		prod_type_lsb;
			__be16		trap_num;
		} PACK_SUFFIX generic;

		struct _notice_vend
		{
			uint8_t		vend_id_msb;
			__be16		vend_id_lsb;
			__be16		dev_id;
		} PACK_SUFFIX vend;
	} g_or_v;

	__be16		issuer_lid;		// 2		8
	__be16		toggle_count;		// 2		10

	union _data_details			// 54		64
	{
		struct _raw_data
		{
			uint8_t	details[54];
		} PACK_SUFFIX raw_data;

		struct _ntc_64_67
		{
			uint8_t		res[6];
			union umad_gid	gid;	// the Node or Multicast Group that came in/out
		} PACK_SUFFIX ntc_64_67;

		struct _ntc_144 {
			__be16		pad1;
			__be16		lid;		// lid where capability mask changed
			__be16		pad2;
			__be32		new_cap_mask;	// new capability mask
		} PACK_SUFFIX ntc_144;

	} data_details;

	union umad_gid			issuer_gid;	// 16		80

} PACK_SUFFIX ib_mad_notice_attr_t;

/****f* IBA Base: Types/ib_gid_get_guid
* NAME
*	ib_gid_get_guid
*
* DESCRIPTION
*	Gets the guid from a GID.
*
* SYNOPSIS
*/
static inline __be64 ib_gid_get_guid(const union umad_gid *const p_gid)
{
	return p_gid->global.interface_id;
}

#endif
