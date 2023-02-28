/*
  SoftwareSerial.cpp (formerly NewSoftSerial.cpp) -
  Multi-instance software serial library for Arduino/Wiring
  -- Interrupt-driven receive and other improvements by ladyada
   (http://ladyada.net)
  -- Tuning, circular buffer, derivation from class Print/Stream,
   multi-instance support, porting to 8MHz processors,
   various optimizations, PROGMEM delay tables, inverse logic and
   direct port writing by Mikal Hart (http://www.arduiniana.org)
  -- Pin change interrupt macros by Paul Stoffregen (http://www.pjrc.com)
  -- 20MHz processor support by Garrett Mace (http://www.macetech.com)
  -- ATmega1280/2560 support by Brett Hagman (http://www.roguerobotics.com/)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  The latest version of this library can always be found at
  http://arduiniana.org.
*/

// When set, _DEBUG co-opts pins 11 and 13 for debugging with an
// oscilloscope or logic analyzer.  Beware: it also slightly modifies
// the bit times, so don't rely on it too much at high baud rates
//
// Includes
//
#include <Arduino.h>
#include "SoftwareSerial.h"

#if defined(BEENEXT) ||  BLYNKGO_USE_BEENEXT || BLYNKGO_USE_SOFTWARESERIAL
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------  ESP
//--------------------------------------------------------------------------------
#if defined(ESP8266) || defined(ESP32)

#ifndef ESP32
uint32_t SoftwareSerial::m_savedPS = 0;
#else
portMUX_TYPE SoftwareSerial::m_interruptsMux = portMUX_INITIALIZER_UNLOCKED;
#endif

inline void IRAM_ATTR SoftwareSerial::disableInterrupts()
{
#ifndef ESP32
  m_savedPS = xt_rsil(15);
#else
  taskENTER_CRITICAL(&m_interruptsMux);
#endif
}

inline void IRAM_ATTR SoftwareSerial::restoreInterrupts()
{
#ifndef ESP32
  xt_wsr_ps(m_savedPS);
#else
  taskEXIT_CRITICAL(&m_interruptsMux);
#endif
}

constexpr uint8_t BYTE_ALL_BITS_SET = ~static_cast<uint8_t>(0);

SoftwareSerial::SoftwareSerial() {
  m_isrOverflow = false;
  m_rxGPIOPullupEnabled = true;
}

SoftwareSerial::SoftwareSerial(int8_t rxPin, int8_t txPin, bool invert)
{
  m_isrOverflow = false;
  m_rxGPIOPullupEnabled = true;
  m_rxPin = rxPin;
  m_txPin = txPin;
  m_invert = invert;
}

SoftwareSerial::~SoftwareSerial() {
  end();
}

bool SoftwareSerial::isValidGPIOpin(int8_t pin) {
#if defined(ESP8266)
  return (pin >= 0 && pin <= 16) && !isFlashInterfacePin(pin);
#elif defined(ESP32)
  // Remove the strapping pins as defined in the datasheets, they affect bootup and other critical operations
  // Remmove the flash memory pins on related devices, since using these causes memory access issues.
#ifdef CONFIG_IDF_TARGET_ESP32
  // Datasheet https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf,
  // Pinout    https://docs.espressif.com/projects/esp-idf/en/latest/esp32/_images/esp32-devkitC-v4-pinout.jpg
  return (pin == 1) || (pin >= 3 && pin <= 5) ||
         (pin >= 12 && pin <= 15) ||
         (!psramFound() && pin >= 16 && pin <= 17) ||
         (pin >= 18 && pin <= 19) ||
         (pin >= 21 && pin <= 23) || (pin >= 25 && pin <= 27) || (pin >= 32 && pin <= 39);
#elif CONFIG_IDF_TARGET_ESP32S2
  // Datasheet https://www.espressif.com/sites/default/files/documentation/esp32-s2_datasheet_en.pdf,
  // Pinout    https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/_images/esp32-s2_saola1-pinout.jpg
  return (pin >= 1 && pin <= 21) || (pin >= 33 && pin <= 44);
#elif CONFIG_IDF_TARGET_ESP32C3
  // Datasheet https://www.espressif.com/sites/default/files/documentation/esp32-c3_datasheet_en.pdf,
  // Pinout    https://docs.espressif.com/projects/esp-idf/en/latest/esp32c3/_images/esp32-c3-devkitm-1-v1-pinout.jpg
  return (pin >= 0 && pin <= 1) || (pin >= 3 && pin <= 7) || (pin >= 18 && pin <= 21);
#else
  return pin >= 0;
#endif
#else
  return pin >= 0;
#endif
}

bool SoftwareSerial::isValidRxGPIOpin(int8_t pin) {
  return isValidGPIOpin(pin)
#if defined(ESP8266)
         && (pin != 16)
#endif
         ;
}

bool SoftwareSerial::isValidTxGPIOpin(int8_t pin) {
  return isValidGPIOpin(pin)
#if defined(ESP32)
#ifdef CONFIG_IDF_TARGET_ESP32
         && (pin < 34)
#elif CONFIG_IDF_TARGET_ESP32S2
         && (pin <= 45)
#elif CONFIG_IDF_TARGET_ESP32C3
         // no restrictions
#endif
#endif
         ;
}

bool SoftwareSerial::hasRxGPIOPullUp(int8_t pin) {
#if defined(ESP32)
  return !(pin >= 34 && pin <= 39);
#else
  (void)pin;
  return true;
#endif
}

void SoftwareSerial::setRxGPIOPullUp() {
  if (m_rxValid) {
    pinMode(m_rxPin, hasRxGPIOPullUp(m_rxPin) && m_rxGPIOPullupEnabled ? INPUT_PULLUP : INPUT);
  }
}

void SoftwareSerial::begin(uint32_t baud, SoftwareSerialConfig config,
                           int8_t rxPin, int8_t txPin,
                           bool invert, int bufCapacity, int isrBufCapacity) {
  if (-1 != rxPin) m_rxPin = rxPin;
  if (-1 != txPin) m_txPin = txPin;
  m_oneWire = (m_rxPin == m_txPin);
  m_invert = invert;
  m_dataBits = 5 + (config & 07);
  m_parityMode = static_cast<SoftwareSerialParity>(config & 070);
  m_stopBits = 1 + ((config & 0300) ? 1 : 0);
  m_pduBits = m_dataBits + static_cast<bool>(m_parityMode) + m_stopBits;
  m_bitCycles = (ESP.getCpuFreqMHz() * 1000000UL + baud / 2) / baud;
  m_intTxEnabled = true;
  if (isValidRxGPIOpin(m_rxPin)) {
    m_rxReg = portInputRegister(digitalPinToPort(m_rxPin));
    m_rxBitMask = digitalPinToBitMask(m_rxPin);
    m_buffer.reset(new circular_queue<uint8_t>((bufCapacity > 0) ? bufCapacity : 64));
    if (m_parityMode)
    {
      m_parityBuffer.reset(new circular_queue<uint8_t>((m_buffer->capacity() + 7) / 8));
      m_parityInPos = m_parityOutPos = 1;
    }
    m_isrBuffer.reset(new circular_queue<uint32_t, SoftwareSerial*>((isrBufCapacity > 0) ?
                      isrBufCapacity : m_buffer->capacity() * (2 + m_dataBits + static_cast<bool>(m_parityMode))));
    if (m_buffer && (!m_parityMode || m_parityBuffer) && m_isrBuffer) {
      m_rxValid = true;
      setRxGPIOPullUp();
    }
  }
  if (isValidTxGPIOpin(m_txPin)) {
#if !defined(ESP8266)
    m_txReg = portOutputRegister(digitalPinToPort(m_txPin));
#endif
    m_txBitMask = digitalPinToBitMask(m_txPin);
    m_txValid = true;
    if (!m_oneWire) {
      pinMode(m_txPin, OUTPUT);
      digitalWrite(m_txPin, !m_invert);
    }
  }
  if (!m_rxEnabled) {
    enableRx(true);
  }
}

void SoftwareSerial::end()
{
  enableRx(false);
  m_txValid = false;
  if (m_buffer) {
    m_buffer.reset();
  }
  m_parityBuffer.reset();
  if (m_isrBuffer) {
    m_isrBuffer.reset();
  }
}

uint32_t SoftwareSerial::baudRate() {
  return ESP.getCpuFreqMHz() * 1000000UL / m_bitCycles;
}

void SoftwareSerial::setTransmitEnablePin(int8_t txEnablePin) {
  if (isValidTxGPIOpin(txEnablePin)) {
    m_txEnableValid = true;
    m_txEnablePin = txEnablePin;
    pinMode(m_txEnablePin, OUTPUT);
    digitalWrite(m_txEnablePin, LOW);
  }
  else {
    m_txEnableValid = false;
  }
}

void SoftwareSerial::enableIntTx(bool on) {
  m_intTxEnabled = on;
}

void SoftwareSerial::enableRxGPIOPullup(bool on) {
  m_rxGPIOPullupEnabled = on;
  setRxGPIOPullUp();
}

void SoftwareSerial::enableTx(bool on) {
  if (m_txValid && m_oneWire) {
    if (on) {
      enableRx(false);
      pinMode(m_txPin, OUTPUT);
      digitalWrite(m_txPin, !m_invert);
    }
    else {
      setRxGPIOPullUp();
      enableRx(true);
    }
  }
}

void SoftwareSerial::enableRx(bool on) {
  if (m_rxValid) {
    if (on) {
      m_rxLastBit = m_pduBits - 1;
      // Init to stop bit level and current cycle
      m_isrLastCycle = (ESP.getCycleCount() | 1) ^ m_invert;
      if (m_bitCycles >= (ESP.getCpuFreqMHz() * 1000000UL) / 74880UL)
        attachInterruptArg(digitalPinToInterrupt(m_rxPin), reinterpret_cast<void (*)(void*)>(rxBitISR), this, CHANGE);
      else
        attachInterruptArg(digitalPinToInterrupt(m_rxPin), reinterpret_cast<void (*)(void*)>(rxBitSyncISR), this, m_invert ? RISING : FALLING);
    }
    else {
      detachInterrupt(digitalPinToInterrupt(m_rxPin));
    }
    m_rxEnabled = on;
  }
}

int SoftwareSerial::read() {
  if (!m_rxValid) {
    return -1;
  }
  if (!m_buffer->available()) {
    rxBits();
    if (!m_buffer->available()) {
      return -1;
    }
  }
  auto val = m_buffer->pop();
  if (m_parityBuffer)
  {
    m_lastReadParity = m_parityBuffer->peek() & m_parityOutPos;
    m_parityOutPos <<= 1;
    if (!m_parityOutPos)
    {
      m_parityOutPos = 1;
      m_parityBuffer->pop();
    }
  }
  return val;
}

int SoftwareSerial::read(uint8_t* buffer, size_t size) {
  if (!m_rxValid) {
    return 0;
  }
  int avail;
  if (0 == (avail = m_buffer->pop_n(buffer, size))) {
    rxBits();
    avail = m_buffer->pop_n(buffer, size);
  }
  if (!avail) return 0;
  if (m_parityBuffer) {
    uint32_t parityBits = avail;
    while (m_parityOutPos >>= 1) ++parityBits;
    m_parityOutPos = (1 << (parityBits % 8));
    m_parityBuffer->pop_n(nullptr, parityBits / 8);
  }
  return avail;
}

size_t SoftwareSerial::readBytes(uint8_t* buffer, size_t size) {
  if (!m_rxValid || !size) {
    return 0;
  }
  size_t count = 0;
  auto start = millis();
  do {
    auto readCnt = read(&buffer[count], size - count);
    count += readCnt;
    if (count >= size) break;
    if (readCnt) start = millis();
    else optimistic_yield(1000UL);
  } while (millis() - start < _timeout);
  return count;
}

int SoftwareSerial::available() {
  if (!m_rxValid) {
    return 0;
  }
  rxBits();
  int avail = m_buffer->available();
  if (!avail) {
    optimistic_yield(10000UL);
  }
  return avail;
}

void SoftwareSerial::lazyDelay() {
  // Reenable interrupts while delaying to avoid other tasks piling up
  if (!m_intTxEnabled) {
    restoreInterrupts();
  }
  const auto expired = ESP.getCycleCount() - m_periodStart;
  const int32_t remaining = m_periodDuration - expired;
  const int32_t ms = remaining > 0 ? remaining / 1000L / static_cast<int32_t>(ESP.getCpuFreqMHz()) : 0;
  if (ms > 0)
  {
    delay(ms);
  }
  else
  {
    optimistic_yield(10000UL);
  }
  // Disable interrupts again if applicable
  if (!m_intTxEnabled) {
    disableInterrupts();
  }
  preciseDelay();
}

void IRAM_ATTR SoftwareSerial::preciseDelay() {
  while ((ESP.getCycleCount() - m_periodStart) < m_periodDuration) {}
  m_periodDuration = 0;
  m_periodStart = ESP.getCycleCount();
}

void IRAM_ATTR SoftwareSerial::writePeriod(
  uint32_t dutyCycle, uint32_t offCycle, bool withStopBit) {
  preciseDelay();
  if (dutyCycle)
  {
#if defined(ESP8266)
    if (16 == m_txPin) {
      GP16O = 1;
    }
    else {
      GPOS = m_txBitMask;
    }
#else
    *m_txReg |= m_txBitMask;
#endif
    m_periodDuration += dutyCycle;
    if (offCycle || (withStopBit && !m_invert)) {
      if (!withStopBit || m_invert) {
        preciseDelay();
      } else {
        lazyDelay();
      }
    }
  }
  if (offCycle)
  {
#if defined(ESP8266)
    if (16 == m_txPin) {
      GP16O = 0;
    }
    else {
      GPOC = m_txBitMask;
    }
#else
    *m_txReg &= ~m_txBitMask;
#endif
    m_periodDuration += offCycle;
    if (withStopBit && m_invert) lazyDelay();
  }
}

size_t SoftwareSerial::write(uint8_t byte) {
  return write(&byte, 1);
}

size_t SoftwareSerial::write(uint8_t byte, SoftwareSerialParity parity) {
  return write(&byte, 1, parity);
}

size_t SoftwareSerial::write(const uint8_t* buffer, size_t size) {
  return write(buffer, size, m_parityMode);
}

size_t IRAM_ATTR SoftwareSerial::write(const uint8_t* buffer, size_t size, SoftwareSerialParity parity) {
  if (m_rxValid) {
    rxBits();
  }
  if (!m_txValid) {
    return -1;
  }

  if (m_txEnableValid) {
    digitalWrite(m_txEnablePin, HIGH);
  }
  // Stop bit: if inverted, LOW, otherwise HIGH
  bool b = !m_invert;
  uint32_t dutyCycle = 0;
  uint32_t offCycle = 0;
  if (!m_intTxEnabled) {
    // Disable interrupts in order to get a clean transmit timing
    disableInterrupts();
  }
  const uint32_t dataMask = ((1UL << m_dataBits) - 1);
  bool withStopBit = true;
  m_periodDuration = 0;
  m_periodStart = ESP.getCycleCount();
  for (size_t cnt = 0; cnt < size; ++cnt) {
    uint8_t byte = pgm_read_byte(buffer + cnt) & dataMask;
    // push LSB start-data-parity-stop bit pattern into uint32_t
    // Stop bits: HIGH
    uint32_t word = ~0UL;
    // inverted parity bit, performance tweak for xor all-bits-set word
    if (parity && m_parityMode)
    {
      uint32_t parityBit;
      switch (parity)
      {
        case SWSERIAL_PARITY_EVEN:
          // from inverted, so use odd parity
          parityBit = byte;
          parityBit ^= parityBit >> 4;
          parityBit &= 0xf;
          parityBit = (0x9669 >> parityBit) & 1;
          break;
        case SWSERIAL_PARITY_ODD:
          // from inverted, so use even parity
          parityBit = byte;
          parityBit ^= parityBit >> 4;
          parityBit &= 0xf;
          parityBit = (0x6996 >> parityBit) & 1;
          break;
        case SWSERIAL_PARITY_MARK:
          parityBit = 0;
          break;
        case SWSERIAL_PARITY_SPACE:
        // suppresses warning parityBit uninitialized
        default:
          parityBit = 1;
          break;
      }
      word ^= parityBit;
    }
    word <<= m_dataBits;
    word |= byte;
    // Start bit: LOW
    word <<= 1;
    if (m_invert) word = ~word;
    for (int i = 0; i <= m_pduBits; ++i) {
      bool pb = b;
      b = word & (1UL << i);
      if (!pb && b) {
        writePeriod(dutyCycle, offCycle, withStopBit);
        withStopBit = false;
        dutyCycle = offCycle = 0;
      }
      if (b) {
        dutyCycle += m_bitCycles;
      }
      else {
        offCycle += m_bitCycles;
      }
    }
    withStopBit = true;
  }
  writePeriod(dutyCycle, offCycle, true);
  if (!m_intTxEnabled) {
    // restore the interrupt state if applicable
    restoreInterrupts();
  }
  if (m_txEnableValid) {
    digitalWrite(m_txEnablePin, LOW);
  }
  return size;
}

void SoftwareSerial::flush() {
  if (!m_rxValid) {
    return;
  }
  m_buffer->flush();
  if (m_parityBuffer)
  {
    m_parityInPos = m_parityOutPos = 1;
    m_parityBuffer->flush();
  }
}

bool SoftwareSerial::overflow() {
  bool res = m_overflow;
  m_overflow = false;
  return res;
}

int SoftwareSerial::peek() {
  if (!m_rxValid) {
    return -1;
  }
  if (!m_buffer->available()) {
    rxBits();
    if (!m_buffer->available()) return -1;
  }
  auto val = m_buffer->peek();
  if (m_parityBuffer) m_lastReadParity = m_parityBuffer->peek() & m_parityOutPos;
  return val;
}

void SoftwareSerial::rxBits() {
#ifdef ESP8266
  if (m_isrOverflow.load()) {
    m_overflow = true;
    m_isrOverflow.store(false);
  }
#else
  if (m_isrOverflow.exchange(false)) {
    m_overflow = true;
  }
#endif

  m_isrBuffer->for_each(m_isrBufferForEachDel);

  // A stop bit can go undetected if leading data bits are at same level
  // and there was also no next start bit yet, so one word may be pending.
  // Check that there was no new ISR data received in the meantime, inserting an
  // extraneous stop level bit out of sequence breaks rx.
  if (m_rxLastBit < m_pduBits - 1) {
    const uint32_t detectionCycles = (m_pduBits - 1 - m_rxLastBit) * m_bitCycles;
    if (!m_isrBuffer->available() && ESP.getCycleCount() - m_isrLastCycle > detectionCycles) {
      // Produce faux stop bit level, prevents start bit maldetection
      // cycle's LSB is repurposed for the level bit
      rxBits(((m_isrLastCycle + detectionCycles) | 1) ^ m_invert);
    }
  }
}

void SoftwareSerial::rxBits(const uint32_t isrCycle) {
  const bool level = (m_isrLastCycle & 1) ^ m_invert;

  // error introduced by edge value in LSB of isrCycle is negligible
  uint32_t cycles = isrCycle - m_isrLastCycle;
  m_isrLastCycle = isrCycle;

  uint32_t bits = cycles / m_bitCycles;
  if (cycles % m_bitCycles > (m_bitCycles >> 1)) ++bits;
  while (bits > 0) {
    // start bit detection
    if (m_rxLastBit >= (m_pduBits - 1)) {
      // leading edge of start bit?
      if (level) break;
      m_rxLastBit = -1;
      --bits;
      continue;
    }
    // data bits
    if (m_rxLastBit < (m_dataBits - 1)) {
      uint8_t dataBits = min(bits, static_cast<uint32_t>(m_dataBits - 1 - m_rxLastBit));
      m_rxLastBit += dataBits;
      bits -= dataBits;
      m_rxCurByte >>= dataBits;
      if (level) {
        m_rxCurByte |= (BYTE_ALL_BITS_SET << (8 - dataBits));
      }
      continue;
    }
    // parity bit
    if (m_parityMode && m_rxLastBit == (m_dataBits - 1)) {
      ++m_rxLastBit;
      --bits;
      m_rxCurParity = level;
      continue;
    }
    // stop bits
    // Store the received value in the buffer unless we have an overflow
    // if not high stop bit level, discard word
    if (bits >= static_cast<uint32_t>(m_pduBits - 1 - m_rxLastBit) && level) {
      m_rxCurByte >>= (sizeof(uint8_t) * 8 - m_dataBits);
      if (!m_buffer->push(m_rxCurByte)) {
        m_overflow = true;
      }
      else {
        if (m_parityBuffer)
        {
          if (m_rxCurParity) {
            m_parityBuffer->pushpeek() |= m_parityInPos;
          }
          else {
            m_parityBuffer->pushpeek() &= ~m_parityInPos;
          }
          m_parityInPos <<= 1;
          if (!m_parityInPos)
          {
            m_parityBuffer->push();
            m_parityInPos = 1;
          }
        }
      }
    }
    m_rxLastBit = m_pduBits - 1;
    // reset to 0 is important for masked bit logic
    m_rxCurByte = 0;
    m_rxCurParity = false;
    break;
  }
}

void IRAM_ATTR SoftwareSerial::rxBitISR(SoftwareSerial* self) {
  uint32_t curCycle = ESP.getCycleCount();
  bool level = *self->m_rxReg & self->m_rxBitMask;

  // Store level and cycle in the buffer unless we have an overflow
  // cycle's LSB is repurposed for the level bit
  if (!self->m_isrBuffer->push((curCycle | 1U) ^ !level)) self->m_isrOverflow.store(true);
}

void IRAM_ATTR SoftwareSerial::rxBitSyncISR(SoftwareSerial* self) {
  uint32_t start = ESP.getCycleCount();
  uint32_t wait = self->m_bitCycles - 172U;

  bool level = self->m_invert;
  // Store level and cycle in the buffer unless we have an overflow
  // cycle's LSB is repurposed for the level bit
  if (!self->m_isrBuffer->push(((start + wait) | 1U) ^ !level)) self->m_isrOverflow.store(true);

  for (uint32_t i = 0; i < self->m_pduBits; ++i) {
    while (ESP.getCycleCount() - start < wait) {};
    wait += self->m_bitCycles;

    // Store level and cycle in the buffer unless we have an overflow
    // cycle's LSB is repurposed for the level bit
    if (static_cast<bool>(*self->m_rxReg & self->m_rxBitMask) != level)
    {
      if (!self->m_isrBuffer->push(((start + wait) | 1U) ^ level)) self->m_isrOverflow.store(true);
      level = !level;
    }
  }
}

void SoftwareSerial::onReceive(Delegate<void(int available), void*> handler) {
  receiveHandler = handler;
}

void SoftwareSerial::perform_work() {
  if (!m_rxValid) {
    return;
  }
  rxBits();
  if (receiveHandler) {
    int avail = m_buffer->available();
    if (avail) {
      receiveHandler(avail);
    }
  }
}


//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------  AVR
//--------------------------------------------------------------------------------
#else

#define _DEBUG 0
#define _DEBUG_PIN1 11
#define _DEBUG_PIN2 13
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay_basic.h>

//
// Statics
//
SoftwareSerial *SoftwareSerial::active_object = 0;
uint8_t SoftwareSerial::_receive_buffer[_SS_MAX_RX_BUFF];
volatile uint8_t SoftwareSerial::_receive_buffer_tail = 0;
volatile uint8_t SoftwareSerial::_receive_buffer_head = 0;

//
// Debugging
//
// This function generates a brief pulse
// for debugging or measuring on an oscilloscope.
#if _DEBUG
inline void DebugPulse(uint8_t pin, uint8_t count)
{
  volatile uint8_t *pport = portOutputRegister(digitalPinToPort(pin));

  uint8_t val = *pport;
  while (count--)
  {
    *pport = val | digitalPinToBitMask(pin);
    *pport = val;
  }
}
#else
inline void DebugPulse(uint8_t, uint8_t) {}
#endif

//
// Private methods
//

/* static */
inline void SoftwareSerial::tunedDelay(uint16_t delay) {
  _delay_loop_2(delay);
}

// This function sets the current object as the "listening"
// one and returns true if it replaces another
bool SoftwareSerial::listen()
{
  if (!_rx_delay_stopbit)
    return false;

  if (active_object != this)
  {
    if (active_object)
      active_object->stopListening();

    _buffer_overflow = false;
    _receive_buffer_head = _receive_buffer_tail = 0;
    active_object = this;

    setRxIntMsk(true);
    return true;
  }

  return false;
}

// Stop listening. Returns true if we were actually listening.
bool SoftwareSerial::stopListening()
{
  if (active_object == this)
  {
    setRxIntMsk(false);
    active_object = NULL;
    return true;
  }
  return false;
}

//
// The receive routine called by the interrupt handler
//
void SoftwareSerial::recv()
{

#if GCC_VERSION < 40302
  // Work-around for avr-gcc 4.3.0 OSX version bug
  // Preserve the registers that the compiler misses
  // (courtesy of Arduino forum user *etracer*)
  asm volatile(
    "push r18 \n\t"
    "push r19 \n\t"
    "push r20 \n\t"
    "push r21 \n\t"
    "push r22 \n\t"
    "push r23 \n\t"
    "push r26 \n\t"
    "push r27 \n\t"
    ::);
#endif

  uint8_t d = 0;

  // If RX line is high, then we don't see any start bit
  // so interrupt is probably not for us
  if (_inverse_logic ? rx_pin_read() : !rx_pin_read())
  {
    // Disable further interrupts during reception, this prevents
    // triggering another interrupt directly after we return, which can
    // cause problems at higher baudrates.
    setRxIntMsk(false);

    // Wait approximately 1/2 of a bit width to "center" the sample
    tunedDelay(_rx_delay_centering);
    DebugPulse(_DEBUG_PIN2, 1);

    // Read each of the 8 bits
    for (uint8_t i = 8; i > 0; --i)
    {
      tunedDelay(_rx_delay_intrabit);
      d >>= 1;
      DebugPulse(_DEBUG_PIN2, 1);
      if (rx_pin_read())
        d |= 0x80;
    }

    if (_inverse_logic)
      d = ~d;

    // if buffer full, set the overflow flag and return
    uint8_t next = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
    if (next != _receive_buffer_head)
    {
      // save new data in buffer: tail points to where byte goes
      _receive_buffer[_receive_buffer_tail] = d; // save new byte
      _receive_buffer_tail = next;
    }
    else
    {
      DebugPulse(_DEBUG_PIN1, 1);
      _buffer_overflow = true;
    }

    // skip the stop bit
    tunedDelay(_rx_delay_stopbit);
    DebugPulse(_DEBUG_PIN1, 1);

    // Re-enable interrupts when we're sure to be inside the stop bit
    setRxIntMsk(true);

  }

#if GCC_VERSION < 40302
  // Work-around for avr-gcc 4.3.0 OSX version bug
  // Restore the registers that the compiler misses
  asm volatile(
    "pop r27 \n\t"
    "pop r26 \n\t"
    "pop r23 \n\t"
    "pop r22 \n\t"
    "pop r21 \n\t"
    "pop r20 \n\t"
    "pop r19 \n\t"
    "pop r18 \n\t"
    ::);
#endif
}

uint8_t SoftwareSerial::rx_pin_read()
{
  return *_receivePortRegister & _receiveBitMask;
}

//
// Interrupt handling
//

/* static */
inline void SoftwareSerial::handle_interrupt()
{
  if (active_object)
  {
    active_object->recv();
  }
}

#if defined(PCINT0_vect)
ISR(PCINT0_vect)
{
  SoftwareSerial::handle_interrupt();
}
#endif

#if defined(PCINT1_vect)
ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
#endif

#if defined(PCINT2_vect)
ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));
#endif

#if defined(PCINT3_vect)
ISR(PCINT3_vect, ISR_ALIASOF(PCINT0_vect));
#endif

//
// Constructor
//
SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic /* = false */) :
  _rx_delay_centering(0),
  _rx_delay_intrabit(0),
  _rx_delay_stopbit(0),
  _tx_delay(0),
  _buffer_overflow(false),
  _inverse_logic(inverse_logic)
{
  setTX(transmitPin);
  setRX(receivePin);
}

//
// Destructor
//
SoftwareSerial::~SoftwareSerial()
{
  end();
}

void SoftwareSerial::setTX(uint8_t tx)
{
  // First write, then set output. If we do this the other way around,
  // the pin would be output low for a short while before switching to
  // output high. Now, it is input with pullup for a short while, which
  // is fine. With inverse logic, either order is fine.
  digitalWrite(tx, _inverse_logic ? LOW : HIGH);
  pinMode(tx, OUTPUT);
  _transmitBitMask = digitalPinToBitMask(tx);
  uint8_t port = digitalPinToPort(tx);
  _transmitPortRegister = portOutputRegister(port);
}

void SoftwareSerial::setRX(uint8_t rx)
{
  pinMode(rx, INPUT);
  if (!_inverse_logic)
    digitalWrite(rx, HIGH);  // pullup for normal logic!
  _receivePin = rx;
  _receiveBitMask = digitalPinToBitMask(rx);
  uint8_t port = digitalPinToPort(rx);
  _receivePortRegister = portInputRegister(port);
}

uint16_t SoftwareSerial::subtract_cap(uint16_t num, uint16_t sub) {
  if (num > sub)
    return num - sub;
  else
    return 1;
}

//
// Public methods
//

void SoftwareSerial::begin(long speed)
{
  _rx_delay_centering = _rx_delay_intrabit = _rx_delay_stopbit = _tx_delay = 0;

  // Precalculate the various delays, in number of 4-cycle delays
  uint16_t bit_delay = (F_CPU / speed) / 4;

  // 12 (gcc 4.8.2) or 13 (gcc 4.3.2) cycles from start bit to first bit,
  // 15 (gcc 4.8.2) or 16 (gcc 4.3.2) cycles between bits,
  // 12 (gcc 4.8.2) or 14 (gcc 4.3.2) cycles from last bit to stop bit
  // These are all close enough to just use 15 cycles, since the inter-bit
  // timings are the most critical (deviations stack 8 times)
  _tx_delay = subtract_cap(bit_delay, 15 / 4);

  // Only setup rx when we have a valid PCINT for this pin
  if (digitalPinToPCICR((int8_t)_receivePin)) {
#if GCC_VERSION > 40800
    // Timings counted from gcc 4.8.2 output. This works up to 115200 on
    // 16Mhz and 57600 on 8Mhz.
    //
    // When the start bit occurs, there are 3 or 4 cycles before the
    // interrupt flag is set, 4 cycles before the PC is set to the right
    // interrupt vector address and the old PC is pushed on the stack,
    // and then 75 cycles of instructions (including the RJMP in the
    // ISR vector table) until the first delay. After the delay, there
    // are 17 more cycles until the pin value is read (excluding the
    // delay in the loop).
    // We want to have a total delay of 1.5 bit time. Inside the loop,
    // we already wait for 1 bit time - 23 cycles, so here we wait for
    // 0.5 bit time - (71 + 18 - 22) cycles.
    _rx_delay_centering = subtract_cap(bit_delay / 2, (4 + 4 + 75 + 17 - 23) / 4);

    // There are 23 cycles in each loop iteration (excluding the delay)
    _rx_delay_intrabit = subtract_cap(bit_delay, 23 / 4);

    // There are 37 cycles from the last bit read to the start of
    // stopbit delay and 11 cycles from the delay until the interrupt
    // mask is enabled again (which _must_ happen during the stopbit).
    // This delay aims at 3/4 of a bit time, meaning the end of the
    // delay will be at 1/4th of the stopbit. This allows some extra
    // time for ISR cleanup, which makes 115200 baud at 16Mhz work more
    // reliably
    _rx_delay_stopbit = subtract_cap(bit_delay * 3 / 4, (37 + 11) / 4);
#else // Timings counted from gcc 4.3.2 output
    // Note that this code is a _lot_ slower, mostly due to bad register
    // allocation choices of gcc. This works up to 57600 on 16Mhz and
    // 38400 on 8Mhz.
    _rx_delay_centering = subtract_cap(bit_delay / 2, (4 + 4 + 97 + 29 - 11) / 4);
    _rx_delay_intrabit = subtract_cap(bit_delay, 11 / 4);
    _rx_delay_stopbit = subtract_cap(bit_delay * 3 / 4, (44 + 17) / 4);
#endif


    // Enable the PCINT for the entire port here, but never disable it
    // (others might also need it, so we disable the interrupt by using
    // the per-pin PCMSK register).
    *digitalPinToPCICR((int8_t)_receivePin) |= _BV(digitalPinToPCICRbit(_receivePin));
    // Precalculate the pcint mask register and value, so setRxIntMask
    // can be used inside the ISR without costing too much time.
    _pcint_maskreg = digitalPinToPCMSK(_receivePin);
    _pcint_maskvalue = _BV(digitalPinToPCMSKbit(_receivePin));

    tunedDelay(_tx_delay); // if we were low this establishes the end
  }

#if _DEBUG
  pinMode(_DEBUG_PIN1, OUTPUT);
  pinMode(_DEBUG_PIN2, OUTPUT);
#endif

  listen();
}

void SoftwareSerial::begin(unsigned long baud, uint8_t rx, uint8_t tx) { //BEENEXT
  setTX(tx);
  setRX(rx);
  this->begin(baud);
}

void SoftwareSerial::setRxIntMsk(bool enable)
{
  if (enable)
    *_pcint_maskreg |= _pcint_maskvalue;
  else
    *_pcint_maskreg &= ~_pcint_maskvalue;
}

void SoftwareSerial::end()
{
  stopListening();
}


// Read data from buffer
int SoftwareSerial::read()
{
  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  uint8_t d = _receive_buffer[_receive_buffer_head]; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  return d;
}

int SoftwareSerial::available()
{
  if (!isListening())
    return 0;

  return ((unsigned int)(_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head)) % _SS_MAX_RX_BUFF;
}

size_t SoftwareSerial::write(uint8_t b)
{
  if (_tx_delay == 0) {
    setWriteError();
    return 0;
  }

  // By declaring these as local variables, the compiler will put them
  // in registers _before_ disabling interrupts and entering the
  // critical timing sections below, which makes it a lot easier to
  // verify the cycle timings
  volatile uint8_t *reg = _transmitPortRegister;
  uint8_t reg_mask = _transmitBitMask;
  uint8_t inv_mask = ~_transmitBitMask;
  uint8_t oldSREG = SREG;
  bool inv = _inverse_logic;
  uint16_t delay = _tx_delay;

  if (inv)
    b = ~b;

  cli();  // turn off interrupts for a clean txmit

  // Write the start bit
  if (inv)
    *reg |= reg_mask;
  else
    *reg &= inv_mask;

  tunedDelay(delay);

  // Write each of the 8 bits
  for (uint8_t i = 8; i > 0; --i)
  {
    if (b & 1) // choose bit
      *reg |= reg_mask; // send 1
    else
      *reg &= inv_mask; // send 0

    tunedDelay(delay);
    b >>= 1;
  }

  // restore pin to natural state
  if (inv)
    *reg &= inv_mask;
  else
    *reg |= reg_mask;

  SREG = oldSREG; // turn interrupts back on
  tunedDelay(_tx_delay);

  return 1;
}

void SoftwareSerial::flush()
{
  // There is no tx buffering, simply return
}

int SoftwareSerial::peek()
{
  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  return _receive_buffer[_receive_buffer_head];
}

#endif //#if defined(ESP8266) || defined(ESP32)
#endif //#if defined(BEENEXT) || BLYNKGO_USE_BEENEXT || BLYNKGO_USE_SOFTWARESERIAL
