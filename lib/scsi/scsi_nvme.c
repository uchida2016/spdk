/*-
 *   BSD LICENSE
 *
 *   Copyright (c) 2016 FUJITSU LIMITED, All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "scsi_internal.h"
#include "spdk/nvme_spec.h"

void
spdk_scsi_nvme_translate(struct spdk_bdev_io *bdev_io, int *sc, int *sk,
			    int *asc, int *ascq)
{
	int nvme_sct = bdev_io->error.nvme.sct;
	int nvme_sc = bdev_io->error.nvme.sc;

	switch (nvme_sct) {
	case SPDK_NVME_SCT_GENERIC:
		switch (nvme_sc) {
		case SPDK_NVME_SC_SUCCESS:
			*sc   = SPDK_SCSI_STATUS_GOOD;
			*sk   = SPDK_SCSI_SENSE_NO_SENSE;
			*asc  = SPDK_SCSI_ASC_NO_ADDITIONAL_SENSE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_INVALID_OPCODE:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_INVALID_COMMAND_OPERATION_CODE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_INVALID_FIELD:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_INVALID_FIELD_IN_CDB;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_DATA_TRANSFER_ERROR:
		case SPDK_NVME_SC_CAPACITY_EXCEEDED:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_MEDIUM_ERROR;
			*asc  = SPDK_SCSI_ASC_NO_ADDITIONAL_SENSE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_ABORTED_POWER_LOSS:
			*sc   = SPDK_SCSI_STATUS_TASK_ABORTED;
			*sk   = SPDK_SCSI_SENSE_ABORTED_COMMAND;
			*asc  = SPDK_SCSI_ASC_WARNING;
			*ascq = SPDK_SCSI_ASCQ_POWER_LOSS_EXPECTED;
			break;
		case SPDK_NVME_SC_INTERNAL_DEVICE_ERROR:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_HARDWARE_ERROR;
			*asc  = SPDK_SCSI_ASC_INTERNAL_TARGET_FAILURE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_ABORTED_BY_REQUEST:
		case SPDK_NVME_SC_ABORTED_SQ_DELETION:
		case SPDK_NVME_SC_ABORTED_FAILED_FUSED:
		case SPDK_NVME_SC_ABORTED_MISSING_FUSED:
			*sc   = SPDK_SCSI_STATUS_TASK_ABORTED;
			*sk   = SPDK_SCSI_SENSE_ABORTED_COMMAND;
			*asc  = SPDK_SCSI_ASC_NO_ADDITIONAL_SENSE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_INVALID_NAMESPACE_OR_FORMAT:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_ACCESS_DENIED;
			*ascq = SPDK_SCSI_ASCQ_INVALID_LU_IDENTIFIER;
			break;
		case SPDK_NVME_SC_LBA_OUT_OF_RANGE:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_NAMESPACE_NOT_READY:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_NOT_READY;
			*asc  = SPDK_SCSI_ASC_LOGICAL_UNIT_NOT_READY;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_RESERVATION_CONFLICT:
			*sc   = SPDK_SCSI_STATUS_RESERVATION_CONFLICT;
			*sk   = SPDK_SCSI_SENSE_NO_SENSE;
			*asc  = SPDK_SCSI_ASC_NO_ADDITIONAL_SENSE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_COMMAND_ID_CONFLICT:
		case SPDK_NVME_SC_COMMAND_SEQUENCE_ERROR:
		case SPDK_NVME_SC_INVALID_SGL_SEG_DESCRIPTOR:
		case SPDK_NVME_SC_INVALID_NUM_SGL_DESCIRPTORS:
		case SPDK_NVME_SC_DATA_SGL_LENGTH_INVALID:
		case SPDK_NVME_SC_METADATA_SGL_LENGTH_INVALID:
		case SPDK_NVME_SC_SGL_DESCRIPTOR_TYPE_INVALID:
		case SPDK_NVME_SC_INVALID_CONTROLLER_MEM_BUF:
		case SPDK_NVME_SC_INVALID_PRP_OFFSET:
		case SPDK_NVME_SC_ATOMIC_WRITE_UNIT_EXCEEDED:
		case SPDK_NVME_SC_INVALID_SGL_OFFSET:
		case SPDK_NVME_SC_INVALID_SGL_SUBTYPE:
		case SPDK_NVME_SC_HOSTID_INCONSISTENT_FORMAT:
		case SPDK_NVME_SC_KEEP_ALIVE_EXPIRED:
		case SPDK_NVME_SC_KEEP_ALIVE_INVALID:
		case SPDK_NVME_SC_FORMAT_IN_PROGRESS:
		default:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_NO_ADDITIONAL_SENSE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		}
		break;
	case SPDK_NVME_SCT_COMMAND_SPECIFIC:
		switch (nvme_sc) {
		case SPDK_NVME_SC_COMPLETION_QUEUE_INVALID:
		case SPDK_NVME_SC_ABORT_COMMAND_LIMIT_EXCEEDED:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_NO_ADDITIONAL_SENSE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_INVALID_FORMAT:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_FORMAT_COMMAND_FAILED;
			*ascq = SPDK_SCSI_ASCQ_FORMAT_COMMAND_FAILED;
			break;
		case SPDK_NVME_SC_CONFLICTING_ATTRIBUTES:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_INVALID_FIELD_IN_CDB;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_ATTEMPTED_WRITE_TO_RO_PAGE:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_DATA_PROTECT;
			*asc  = SPDK_SCSI_ASC_WRITE_PROTECTED;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_INVALID_QUEUE_IDENTIFIER:
		case SPDK_NVME_SC_MAXIMUM_QUEUE_SIZE_EXCEEDED:
		case SPDK_NVME_SC_ASYNC_EVENT_REQUEST_LIMIT_EXCEEDED:
		case SPDK_NVME_SC_INVALID_FIRMWARE_SLOT:
		case SPDK_NVME_SC_INVALID_FIRMWARE_IMAGE:
		case SPDK_NVME_SC_INVALID_INTERRUPT_VECTOR:
		case SPDK_NVME_SC_INVALID_LOG_PAGE:
		case SPDK_NVME_SC_FIRMWARE_REQ_CONVENTIONAL_RESET:
		case SPDK_NVME_SC_INVALID_QUEUE_DELETION:
		case SPDK_NVME_SC_FEATURE_ID_NOT_SAVEABLE:
		case SPDK_NVME_SC_FEATURE_NOT_CHANGEABLE:
		case SPDK_NVME_SC_FEATURE_NOT_NAMESPACE_SPECIFIC:
		case SPDK_NVME_SC_FIRMWARE_REQ_NVM_RESET:
		case SPDK_NVME_SC_FIRMWARE_REQ_RESET:
		case SPDK_NVME_SC_FIRMWARE_REQ_MAX_TIME_VIOLATION:
		case SPDK_NVME_SC_FIRMWARE_ACTIVATION_PROHIBITED:
		case SPDK_NVME_SC_OVERLAPPING_RANGE:
		case SPDK_NVME_SC_NAMESPACE_INSUFFICIENT_CAPACITY:
		case SPDK_NVME_SC_NAMESPACE_ID_UNAVAILABLE:
		case SPDK_NVME_SC_NAMESPACE_ALREADY_ATTACHED:
		case SPDK_NVME_SC_NAMESPACE_IS_PRIVATE:
		case SPDK_NVME_SC_NAMESPACE_NOT_ATTACHED:
		case SPDK_NVME_SC_THINPROVISIONING_NOT_SUPPORTED:
		case SPDK_NVME_SC_CONTROLLER_LIST_INVALID:
		case SPDK_NVME_SC_INVALID_PROTECTION_INFO:
		default:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_NO_ADDITIONAL_SENSE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		}
		break;
	case SPDK_NVME_SCT_MEDIA_ERROR:
		switch (nvme_sc) {
		case SPDK_NVME_SC_WRITE_FAULTS:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_MEDIUM_ERROR;
			*asc  = SPDK_SCSI_ASC_PERIPHERAL_DEVICE_WRITE_FAULT;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_UNRECOVERED_READ_ERROR:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_MEDIUM_ERROR;
			*asc  = SPDK_SCSI_ASC_UNRECOVERED_READ_ERROR;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_GUARD_CHECK_ERROR:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_MEDIUM_ERROR;
			*asc  = SPDK_SCSI_ASC_LOGICAL_BLOCK_GUARD_CHECK_FAILED;
			*ascq = SPDK_SCSI_ASCQ_LOGICAL_BLOCK_GUARD_CHECK_FAILED;
			break;
		case SPDK_NVME_SC_APPLICATION_TAG_CHECK_ERROR:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_MEDIUM_ERROR;
			*asc  = SPDK_SCSI_ASC_LOGICAL_BLOCK_APP_TAG_CHECK_FAILED;
			*ascq = SPDK_SCSI_ASCQ_LOGICAL_BLOCK_APP_TAG_CHECK_FAILED;
			break;
		case SPDK_NVME_SC_REFERENCE_TAG_CHECK_ERROR:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_MEDIUM_ERROR;
			*asc  = SPDK_SCSI_ASC_LOGICAL_BLOCK_REF_TAG_CHECK_FAILED;
			*ascq = SPDK_SCSI_ASCQ_LOGICAL_BLOCK_REF_TAG_CHECK_FAILED;
			break;
		case SPDK_NVME_SC_COMPARE_FAILURE:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_MISCOMPARE;
			*asc  = SPDK_SCSI_ASC_MISCOMPARE_DURING_VERIFY_OPERATION;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		case SPDK_NVME_SC_ACCESS_DENIED:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_DATA_PROTECT;
			*asc  = SPDK_SCSI_ASC_ACCESS_DENIED;
			*ascq = SPDK_SCSI_ASCQ_NO_ACCESS_RIGHTS;
			break;
		case SPDK_NVME_SC_DEALLOCATED_OR_UNWRITTEN_BLOCK:
		default:
			*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
			*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
			*asc  = SPDK_SCSI_ASC_NO_ADDITIONAL_SENSE;
			*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
			break;
		}
		break;
	case SPDK_NVME_SCT_VENDOR_SPECIFIC:
	default:
		*sc   = SPDK_SCSI_STATUS_CHECK_CONDITION;
		*sk   = SPDK_SCSI_SENSE_ILLEGAL_REQUEST;
		*asc  = SPDK_SCSI_ASC_NO_ADDITIONAL_SENSE;
		*ascq = SPDK_SCSI_ASCQ_CAUSE_NOT_REPORTABLE;
		break;
	}
}
