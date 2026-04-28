use axum::{routing::{get, post}, Json, Router};
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize)]
struct RawTransaction {
    id: String,
    amount: f64,
}

#[tokio::main]
async fn main() {
    let app = Router::new()
        .route("/health", get(|| async { "HADAL_INGRESS_UP" }))
        .route("/ingest", post(handle_ingest));

    let listener = tokio::net::TcpListener::bind("0.0.0.0:8080").await.unwrap();
    println!("📡 Hadal Ingress Online: Port 8080");
    axum::serve(listener, app).await.unwrap();
}

async fn handle_ingest(Json(payload): Json<RawTransaction>) -> Json<serde_json::Value> {
    println!("[HADAL] Ingesting TX: {}", payload.id);
    Json(serde_json::json!({ "received": payload.id, "status": "buffered" }))
}
