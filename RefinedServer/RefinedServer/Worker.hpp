#pragma once
import Utility.Concurrency.Thread;
import Net;

void Worker(util::CancellationToken&& cancel_token, net::CompletionPort& completion_port);

