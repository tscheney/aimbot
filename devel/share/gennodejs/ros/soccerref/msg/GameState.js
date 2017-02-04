// Auto-generated. Do not edit!

// (in-package soccerref.msg)


"use strict";

let _serializer = require('../base_serialize.js');
let _deserializer = require('../base_deserialize.js');
let _finder = require('../find.js');

//-----------------------------------------------------------

class GameState {
  constructor() {
    this.home_score = 0;
    this.away_score = 0;
    this.home_bot_count = 0;
    this.away_bot_count = 0;
    this.remaining_seconds = 0;
    this.play = false;
    this.reset_field = false;
    this.second_half = false;
  }

  static serialize(obj, bufferInfo) {
    // Serializes a message object of type GameState
    // Serialize message field [home_score]
    bufferInfo = _serializer.int32(obj.home_score, bufferInfo);
    // Serialize message field [away_score]
    bufferInfo = _serializer.int32(obj.away_score, bufferInfo);
    // Serialize message field [home_bot_count]
    bufferInfo = _serializer.int32(obj.home_bot_count, bufferInfo);
    // Serialize message field [away_bot_count]
    bufferInfo = _serializer.int32(obj.away_bot_count, bufferInfo);
    // Serialize message field [remaining_seconds]
    bufferInfo = _serializer.int32(obj.remaining_seconds, bufferInfo);
    // Serialize message field [play]
    bufferInfo = _serializer.bool(obj.play, bufferInfo);
    // Serialize message field [reset_field]
    bufferInfo = _serializer.bool(obj.reset_field, bufferInfo);
    // Serialize message field [second_half]
    bufferInfo = _serializer.bool(obj.second_half, bufferInfo);
    return bufferInfo;
  }

  static deserialize(buffer) {
    //deserializes a message object of type GameState
    let tmp;
    let len;
    let data = new GameState();
    // Deserialize message field [home_score]
    tmp = _deserializer.int32(buffer);
    data.home_score = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [away_score]
    tmp = _deserializer.int32(buffer);
    data.away_score = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [home_bot_count]
    tmp = _deserializer.int32(buffer);
    data.home_bot_count = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [away_bot_count]
    tmp = _deserializer.int32(buffer);
    data.away_bot_count = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [remaining_seconds]
    tmp = _deserializer.int32(buffer);
    data.remaining_seconds = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [play]
    tmp = _deserializer.bool(buffer);
    data.play = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [reset_field]
    tmp = _deserializer.bool(buffer);
    data.reset_field = tmp.data;
    buffer = tmp.buffer;
    // Deserialize message field [second_half]
    tmp = _deserializer.bool(buffer);
    data.second_half = tmp.data;
    buffer = tmp.buffer;
    return {
      data: data,
      buffer: buffer
    }
  }

  static datatype() {
    // Returns string type for a message object
    return 'soccerref/GameState';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '04b9504566012ef17377e5d0ae0aba41';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    int32 home_score
    int32 away_score
    int32 home_bot_count
    int32 away_bot_count
    int32 remaining_seconds
    bool play
    bool reset_field
    bool second_half
    `;
  }

};

module.exports = GameState;
