#pragma once
import Utility.Concurrency.Thread;
import Utility.Concurrency.Thread.Unit;
import Net;
import Net.Worker;

void Worker(net::WorkerUnit&& unit, net::CompletionPort& port);
