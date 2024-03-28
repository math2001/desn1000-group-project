function assert(cond: boolean, msg?: string): asserts cond
{
  if (!cond)
    throw new Error("assertion error: " + (msg || ""))
}


function main()
{
  const canvas = document.querySelector('canvas');
  assert(!!canvas);

  const ctx = canvas.getContext('2d');
  assert(!!ctx);

  const ws = new WebSocket('ws://localhost:8080/ws')
  assert(!!ws);
  ws.on('open', () => {
    console.log("hello")
  })
}

