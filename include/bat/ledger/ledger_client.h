/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BAT_LEDGER_LEDGER_CLIENT_H_
#define BAT_LEDGER_LEDGER_CLIENT_H_

#include <memory>

#include "bat/ledger/ledger_callback_handler.h"
#include "bat/ledger/ledger_task_runner.h"
#include "bat/ledger/ledger_url_loader.h"

namespace ledger {

enum URL_METHOD {
  GET = 0,
  PUT = 1,
  POST = 2
};

class LedgerClient {
 public:
  virtual ~LedgerClient() = default;

  // called when the wallet creation has completed
  virtual std::string GenerateGUID() const = 0;
  virtual void OnWalletCreated(Result result) = 0;
  virtual void OnReconcileComplete(Result result,
                                   const std::string& viewing_id) = 0;
  virtual void LoadLedgerState(LedgerCallbackHandler* handler) = 0;
  virtual void LoadPublisherState(LedgerCallbackHandler* handler) = 0;
  virtual void SaveLedgerState(const std::string& ledger_state,
                               LedgerCallbackHandler* handler) = 0;
  virtual void SavePublisherState(const std::string& publisher_state,
                                  LedgerCallbackHandler* handler) = 0;
  virtual std::unique_ptr<ledger::LedgerURLLoader> LoadURL(const std::string& url,
      const std::vector<std::string>& headers,
      const std::string& content,
      const std::string& contentType,
      const ledger::URL_METHOD& method,
      ledger::LedgerCallbackHandler* handler) = 0;
  // RunIOTask and RunTask are temporary workarounds for leveldb
  // and we should replace them with a ledger_client api for reading/writing
  // individual records
  virtual void RunIOTask(std::unique_ptr<LedgerTaskRunner> task) = 0;
  // If any callbacks are made from inside RunIOTask you must use
  // RunTask to return back to the calling thread
  virtual void RunTask(std::unique_ptr<LedgerTaskRunner> task) = 0;
};

}  // namespace ledger

#endif  // BAT_LEDGER_LEDGER_CLIENT_H_