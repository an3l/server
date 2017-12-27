/*****************************************************************************

Copyright (c) 1997, 2016, Oracle and/or its affiliates. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA

*****************************************************************************/

/**************************************************//**
@file include/read0types.h
Cursor read

Created 2/16/1997 Heikki Tuuri
*******************************************************/

#ifndef read0types_h
#define read0types_h

#include <algorithm>
#include "dict0mem.h"

#include "trx0types.h"

// Friend declaration
class MVCC;

/** Read view lists the trx ids of those transactions for which a consistent
read should not see the modifications to the database. */

class ReadView {
public:
	ReadView();
	~ReadView();
	/** Check whether transaction id is valid.
	@param[in]	id		transaction id to check
	@param[in]	name		table name */
	static void check_trx_id_sanity(
		trx_id_t		id,
		const table_name_t&	name);

	/** Check whether the changes by id are visible.
	@param[in]	id	transaction id to check against the view
	@param[in]	name	table name
	@return whether the view sees the modifications of id. */
	bool changes_visible(
		trx_id_t		id,
		const table_name_t&	name) const
		MY_ATTRIBUTE((warn_unused_result))
	{
		if (id < m_up_limit_id || id == m_creator_trx_id) {

			return(true);
		}

		check_trx_id_sanity(id, name);

		if (id >= m_low_limit_id) {

			return(false);

		} else if (m_ids.empty()) {

			return(true);
		}

		return(!std::binary_search(m_ids.begin(), m_ids.end(), id));
	}

	/**
	@param id		transaction to check
	@return true if view sees transaction id */
	bool sees(trx_id_t id) const
	{
		return(id < m_up_limit_id);
	}

	/**
	Mark the view as closed */
	void close()
	{
		ut_ad(m_creator_trx_id != TRX_ID_MAX);
		m_creator_trx_id = TRX_ID_MAX;
	}

	/**
	@return true if the view is closed */
	bool is_closed() const
	{
		return(m_closed);
	}

	/**
	Write the limits to the file.
	@param file		file to write to */
	void print_limits(FILE* file) const
	{
		fprintf(file,
			"Trx read view will not see trx with"
			" id >= " TRX_ID_FMT ", sees < " TRX_ID_FMT "\n",
			m_low_limit_id, m_up_limit_id);
	}

	/**
	@return the low limit no */
	trx_id_t low_limit_no() const
	{
		return(m_low_limit_no);
	}

	/**
	@return the low limit id */
	trx_id_t low_limit_id() const
	{
		return(m_low_limit_id);
	}

	/**
	@return true if there are no transaction ids in the snapshot */
	bool empty() const
	{
		return(m_ids.empty());
	}

#ifdef UNIV_DEBUG
	/**
	@param rhs		view to compare with
	@return truen if this view is less than or equal rhs */
	bool le(const ReadView* rhs) const
	{
		return(m_low_limit_no <= rhs->m_low_limit_no);
	}

	trx_id_t up_limit_id() const
	{
		return(m_up_limit_id);
	}
#endif /* UNIV_DEBUG */
private:
	/**
	Opens a read view where exactly the transactions serialized before this
	point in time are seen in the view.
	@param id		Creator transaction id */
	inline void prepare(trx_t *trx);

	/**
	Complete the read view creation */
	inline void complete();

	/**
	Copy state from another view. Must call copy_complete() to finish.
	@param other		view to copy from */
	inline void copy_prepare(const ReadView& other);

	/**
	Complete the copy, insert the creator transaction id into the
	m_trx_ids too and adjust the m_up_limit_id *, if required */
	inline void copy_complete();

	/**
	Set the creator transaction id, existing id must be 0 */
	void creator_trx_id(trx_id_t id)
	{
		ut_ad(m_creator_trx_id == 0);
		m_creator_trx_id = id;
	}

	friend class MVCC;

private:
	// Disable copying
	ReadView(const ReadView&);
	ReadView& operator=(const ReadView&);

private:
	/** The read should not see any transaction with trx id >= this
	value. In other words, this is the "high water mark". */
	trx_id_t	m_low_limit_id;

	/** The read should see all trx ids which are strictly
	smaller (<) than this value.  In other words, this is the
	low water mark". */
	trx_id_t	m_up_limit_id;

	/** trx id of creating transaction, set to TRX_ID_MAX for free
	views. */
	trx_id_t	m_creator_trx_id;

	/** Set of RW transactions that was active when this snapshot
	was taken */
	trx_ids_t	m_ids;

	/** The view does not need to see the undo logs for transactions
	whose transaction number is strictly smaller (<) than this value:
	they can be removed in purge if not needed by other views */
	trx_id_t	m_low_limit_no;

	/** AC-NL-RO transaction view that has been "closed". */
	bool		m_closed;

	typedef UT_LIST_NODE_T(ReadView) node_t;

	/** List of read views in trx_sys */
	byte		pad1[64 - sizeof(node_t)];
	node_t		m_view_list;
};

#endif
