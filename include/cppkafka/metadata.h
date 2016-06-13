/*
 * Copyright (c) 2016, Matias Fontanini
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef CPPKAFKA_METADATA_H
#define CPPKAFKA_METADATA_H

#include <memory>
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_set>
#include <librdkafka/rdkafka.h>

namespace cppkafka {

/**
 * Represents the metadata for a partition
 */
class PartitionMetadata {
public:
    PartitionMetadata(const rd_kafka_metadata_partition& partition);

    /**
     * Gets the partition id
     */
    uint32_t get_id() const;

    /**
     * Gets the partition error as reported by the broker
     */
    rd_kafka_resp_err_t get_error() const;

    /**
     * Gets the leader broker id
     */
    int32_t get_leader() const;

    /**
     * Gets the replica brokers
     */
    const std::vector<int32_t>& get_replicas() const;

    /**
     * Gets the In Sync Replica Brokers
     */
    const std::vector<int32_t>& get_in_sync_replica_brokers() const;
private:
    int32_t id_;
    rd_kafka_resp_err_t error_;
    int32_t leader_;
    std::vector<int32_t> replicas_;
    std::vector<int32_t> isrs_;
};

/**
 * Represents the metadata for a topic
 */
class TopicMetadata {
public:
    TopicMetadata(const rd_kafka_metadata_topic& topic);

    /**
     * Gets the topic name
     */
    const std::string& get_topic() const;

    /**
     * Gets the topic error
     */
    rd_kafka_resp_err_t get_error() const;

    /**
     * Gets the partitions' metadata
     */
    const std::vector<PartitionMetadata>& get_partitions() const;
private:
    std::string topic_;
    rd_kafka_resp_err_t error_;
    std::vector<PartitionMetadata> partitions_;
};

/**
 * Represents a broker's metadata
 */
class BrokerMetadata {
public:
    BrokerMetadata(const rd_kafka_metadata_broker_t& broker);

    /**
     * Gets the host this broker can be found at
     */
    const std::string& get_host() const;

    /**
     * Gets the broker's id
     */
    int32_t get_id() const;

    /**
     * Gets the broker's port
     */
    uint16_t get_port() const;
private:
    const std::string host_;
    int32_t id_;
    uint16_t port_;
};

/**
 * Represents metadata for brokers, topics and partitions
 */
class Metadata {
public:
    Metadata(const rd_kafka_metadata_t* ptr);

    /**
     * Gets the brokers' metadata
     */
    std::vector<BrokerMetadata> get_brokers() const;
    
    /**
     * Gets the topics' metadata
     */
    std::vector<TopicMetadata> get_topics() const;

    /**
     * Gets metadata for the topics that can be found on the given set
     *
     * \param topics The topic names to be looked up
     */
    std::vector<TopicMetadata> get_topics(const std::unordered_set<std::string>& topics) const;

    /**
     * Gets metadata for topics that start with the given prefix
     *
     * \param prefix The prefix to be looked up
     */
    std::vector<TopicMetadata> get_topics(const std::string& prefix) const;
private:
    using HandlePtr = std::unique_ptr<const rd_kafka_metadata_t, decltype(&rd_kafka_metadata_destroy)>;

    HandlePtr handle_;
};

} // cppkafka

#endif // CPPKAFKA_METADATA_H
