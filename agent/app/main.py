from fastapi import FastAPI
from mcp import McpServer
from pydantic import BaseModel

app = FastAPI(title="Hadal-Core Intelligence")
mcp = McpServer(name="hadal-agent")

class ReconTransaction(BaseModel):
    tx_id: str
    amount_zar: float
    status: str = "pending"

@app.get("/health")
async def health():
    return {"node": "HADAL_AGENT_ACTIVE", "interop": "MCP_READY"}

@app.post("/audit")
async def audit_tx(tx: ReconTransaction):
    return {"tx_id": tx.tx_id, "audit_result": "verified_by_hadal"}
